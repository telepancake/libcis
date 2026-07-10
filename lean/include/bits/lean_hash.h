// lean/include/bits/lean_hash.h — the lean profile's open-hashing engine for
// unordered_map / unordered_set.
//
// Representation (contract, see lean/README.md):
//   The container object is ONE pointer to a malloc-ed CONTROL BLOCK:
//
//     [ hash_control { size; bucket_count; max_load_factor; first } ][ node* buckets[bucket_count] ]
//
//   - nullptr control pointer == empty container (size 0, bucket_count 0). The
//     block is allocated on first insert (or when max_load_factor is set on an
//     empty container). Every path branches on null first.
//   - Layout note: the contract lists "node* head" as a header field; it is
//     realized here as the *next* pointer of an embedded sentinel node_base
//     (`first`). first.next is the head of the single intrusive list that runs
//     through ALL live elements (libc++ layout) so begin() is O(1) and
//     iteration touches only live nodes. first.hash is unused padding.
//   - Each BUCKET entry points at the PREDECESSOR node of that bucket's run
//     (or is null when the bucket is empty). The predecessor of the very first
//     bucket run is `&first` itself. Holding the predecessor makes both insert
//     (splice after predecessor) and erase (no backward scan) O(1). This is the
//     standard libc++ trick.
//   - bucket_count is always a power of two; the bucket index is
//     `cached_hash & (bucket_count - 1)`. Growth doubles at the load-factor
//     threshold.
//
// Type erasure: rehash/relink, bucket-array growth, unlink and the destroy-all
// walk are NON-TEMPLATE inline kernels below that work only on the node header
// (`hnode_base`) plus the cached hashes — a rehash NEVER re-invokes Hash, it
// redistributes by cached_hash. The only templated ("thin") parts are the hash
// thunk, the in-bucket equality walk, and node create/destroy.
//
// Deviations (all deliberate, see lean/README.md and the component report):
//   - No custom allocators: storage is malloc/free; the Allocator template
//     parameter is kept for source compatibility but static_assert-ed to be
//     std::allocator<value_type>.
//   - No node_handle / extract / merge, no pmr aliases.
//   - Local bucket API reduced to bucket_count/load_factor/max_load_factor/
//     rehash/reserve (no bucket(k)/bucket_size(n)/local_iterator).
//   - unordered_multimap / unordered_multiset are not provided in this profile.
//   - Container members are not constexpr.
//
// Target: gcc-10.2, -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti.
#pragma once

#include <cstddef>
#include <cstdlib>
#include <limits>
#include <new>
#include <type_traits>
#include <utility>
#include <memory>
#include <iterator>
#include <bits/lean.h>

namespace std {

template<class T> class allocator;

namespace detail {

// ===========================================================================
// Non-template block layout + kernels (one copy per binary; take no template
// type — they operate purely on the node header and cached hashes).
// ===========================================================================

// Intrusive node header: `next` chains ALL live elements; `hash` is cached so a
// rehash never re-invokes the user's Hash.
struct hnode_base {
    hnode_base* next;
    size_t hash;
};

struct hash_control {
    size_t size;              // live element count
    size_t bucket_count;      // power of two, >= 2 once a block exists
    float max_load_factor;    // never 0
    hnode_base first;         // sentinel: first.next is the head of the list
    // hnode_base* buckets[bucket_count] follows immediately.
};

// The bucket array lives right after the fixed header. sizeof(hash_control) is a
// multiple of alignof(hnode_base*) so the array is correctly aligned.
inline hnode_base** hash_buckets(hash_control* c) noexcept {
    return reinterpret_cast<hnode_base**>(c + 1);
}

// Smallest power of two >= n, but at least 2. Defined so EVERY input is UB-free:
// for n > 2^63 there is no representable power-of-two >= n, so we clamp to the
// highest representable power of two (2^63) rather than evaluating 1 << 64
// (shift-count >= width is UB). The clamped value feeds allocation, which then
// traps on the (astronomically large) request — never a bad shift. constexpr so
// the boundary behaviour is statically checkable (see lean/test/unordered.cpp).
inline constexpr size_t hash_pow2_ceil(size_t n) noexcept {
    if (n < 2)
        return 2;
    constexpr int width = numeric_limits<size_t>::digits;   // 64 on LP64
    int shift = width - __builtin_clzll(static_cast<unsigned long long>(n - 1));
    if (shift >= width)                     // n > 2^(width-1): no larger pow2 fits
        return size_t(1) << (width - 1);    // clamp to 2^63
    return size_t(1) << shift;
}

// ---------------------------------------------------------------------------
// Fat kernels — defined out of line in lean/src/kernels.cpp (one copy per
// system). Declarations only here. hash_rehash_into is internal (only
// hash_set_bucket_count calls it) but kept declared for symmetry. Default
// visibility so the references stay bindable to liblean.a / liblean.so even
// under a hidden-visibility include region (see bits/lean_string.h).
// ---------------------------------------------------------------------------
#pragma GCC visibility push(default)

// Redistribute every live node into the (already-zeroed) bucket array of `ctl`
// using only cached hashes. Unique-key semantics: elements are grouped by
// bucket index; equal keys need no special adjacency here. Ported from libc++
// __hash_table::__do_rehash (UniqueKeys == true branch).
void hash_rehash_into(hash_control* ctl) noexcept;

// Grow/shrink the block to `nbc` buckets (nbc a power of two). `ctl` may be null
// (fresh allocation with default max_load_factor). The element list and the
// header (size / max_load_factor / first.next) are preserved across realloc;
// the bucket array is rebuilt from the cached hashes. Returns the new pointer.
hash_control* hash_set_bucket_count(hash_control* ctl, size_t nbc) noexcept;

// Splice an already-hashed node at the front of its bucket run (unique-key
// perform; ported from libc++ __node_insert_unique_perform).
void hash_link_unique(hash_control* ctl, hnode_base* nd) noexcept;

// Unlink `cn` from the list and fix the bucket predecessors (no backward scan
// beyond the single bucket run). Ported from libc++ __hash_table::remove.
void hash_unlink(hash_control* ctl, hnode_base* cn) noexcept;

// Walk the whole element list destroying each node through `destroy` (a thin
// per-type thunk that runs the value's destructor and frees the node), then
// reset the block to empty. The block itself is NOT freed (clear() keeps it).
void hash_destroy_all(hash_control* ctl, void (*destroy)(hnode_base*)) noexcept;

#pragma GCC visibility pop

// ===========================================================================
// Node (templated thin part: holds the value; created/destroyed per type).
// ===========================================================================

template<class T>
struct hnode : hnode_base {
    // Union so the node's lifetime can begin before the value is constructed
    // (and after it is destroyed), exactly like a slot in a raw buffer.
    union {
        T value;
    };
    hnode() noexcept {}
    ~hnode() {}
};

// ===========================================================================
// Iterator (forward; walks the single intrusive list).
// ===========================================================================

template<class T, bool IsConst>
struct hiter {
    hnode_base* node_ = nullptr;

    using value_type        = T;
    using reference         = conditional_t<IsConst, const T&, T&>;
    using pointer           = conditional_t<IsConst, const T*, T*>;
    using difference_type   = ptrdiff_t;
    using iterator_category = forward_iterator_tag;

    hiter() = default;
    explicit hiter(hnode_base* n) noexcept : node_(n) {}

    // const_iterator is constructible from iterator.
    template<bool C = IsConst, class = enable_if_t<C>>
    hiter(const hiter<T, false>& o) noexcept : node_(o.node_) {}

    reference operator*() const { return static_cast<hnode<T>*>(node_)->value; }
    pointer operator->() const { return std::addressof(static_cast<hnode<T>*>(node_)->value); }

    hiter& operator++() noexcept {
        node_ = node_->next;
        return *this;
    }
    hiter operator++(int) noexcept {
        hiter t = *this;
        node_ = node_->next;
        return t;
    }
};

template<class T, bool C1, bool C2>
inline bool operator==(const hiter<T, C1>& a, const hiter<T, C2>& b) noexcept {
    return a.node_ == b.node_;
}
template<class T, bool C1, bool C2>
inline bool operator!=(const hiter<T, C1>& a, const hiter<T, C2>& b) noexcept {
    return a.node_ != b.node_;
}

// ===========================================================================
// Key/value traits: get_key extracts the lookup key from a stored value
// (whole value for sets; the .first member for map's pair<const Key, T>).
// ===========================================================================

template<class Tp>
struct hash_kv {
    using key_type = Tp;
    static const key_type& get_key(const Tp& v) noexcept { return v; }
};

template<class Key, class T>
struct hash_kv<pair<const Key, T>> {
    using key_type = Key;
    static const key_type& get_key(const pair<const Key, T>& v) noexcept { return v.first; }
};

// ===========================================================================
// Deduction-guide / heterogeneous-lookup helpers (names shared with the base
// hash headers; the base bits/hash_table.h is never co-included under the
// overlay because it only reaches this file through unordered_map/_set).
// ===========================================================================

template<class T, class K, class = void>
inline constexpr bool ht_is_transparent_v = false;
template<class T, class K>
inline constexpr bool ht_is_transparent_v<T, K, void_t<typename T::is_transparent>> = true;

template<class Alloc, class = void, class = void>
inline constexpr bool ht_is_allocator_v = false;
template<class Alloc>
inline constexpr bool ht_is_allocator_v<
    Alloc, void_t<typename Alloc::value_type>,
    void_t<decltype(declval<Alloc&>().allocate(size_t(0)))>> = true;

template<class Iter>
using ht_iter_category_t = typename iterator_traits<Iter>::iterator_category;

template<class InputIt>
using ht_iter_value_type = typename iterator_traits<InputIt>::value_type;
template<class InputIt>
using ht_iter_key_type =
    remove_const_t<typename iterator_traits<InputIt>::value_type::first_type>;
template<class InputIt>
using ht_iter_mapped_type = typename iterator_traits<InputIt>::value_type::second_type;
template<class InputIt>
using ht_iter_to_alloc_type =
    pair<const typename iterator_traits<InputIt>::value_type::first_type,
         typename iterator_traits<InputIt>::value_type::second_type>;

// can_extract_key: lets emplace_unique(x) skip constructing a node when x is
// already a value/key and the key is present. Ported from libc++.
struct extract_key_fail_tag {};
struct extract_key_self_tag {};
struct extract_key_first_tag {};

template<class ValTy, class Key, class RawValTy = remove_const_t<remove_reference_t<ValTy>>>
struct ht_can_extract_key
    : conditional_t<is_same_v<RawValTy, Key>, extract_key_self_tag, extract_key_fail_tag> {};

template<class Pair, class Key, class First, class Second>
struct ht_can_extract_key<Pair, Key, pair<First, Second>>
    : conditional_t<is_same_v<remove_const_t<First>, Key>, extract_key_first_tag,
                    extract_key_fail_tag> {};

template<class ValTy, class Key, class ContainerValueTy,
         class RawValTy = remove_const_t<remove_reference_t<ValTy>>>
struct ht_can_extract_map_key : bool_constant<is_same_v<RawValTy, Key>> {};
template<class ValTy, class Key, class RawValTy>
struct ht_can_extract_map_key<ValTy, Key, Key, RawValTy> : false_type {};

} // namespace detail

// ===========================================================================
// hash_table — the templated engine unordered_map / unordered_set build on.
// Stores ONE pointer plus [[no_unique_address]] Hash/Equal, so an empty
// Hash/Equal (the common case) yields sizeof == sizeof(void*).
// ===========================================================================

template<class T, class Hash, class Equal, class Alloc>
class hash_table {
public:
    using value_type      = T;
    using hasher          = Hash;
    using key_equal        = Equal;
    using allocator_type  = Alloc;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    using iterator       = detail::hiter<T, false>;
    using const_iterator = detail::hiter<T, true>;

    using node = detail::hnode<T>;

private:
    using base = detail::hnode_base;
    using ctrl = detail::hash_control;
    using kv   = detail::hash_kv<T>;

    ctrl* ctl_ = nullptr;
    [[no_unique_address]] Hash hasher_;
    [[no_unique_address]] Equal eq_;

    static T& node_value(base* n) noexcept { return static_cast<node*>(n)->value; }

    static void destroy_node(base* n) noexcept {
        node* p = static_cast<node*>(n);
        p->value.~T();
        ::free(p);
    }

    template<class... Args>
    node* create_node(size_t hash, Args&&... args) {
        void* mem = ::malloc(sizeof(node));
        if (mem == nullptr)
            __builtin_trap();
        node* n = static_cast<node*>(mem);
        n->next = nullptr;
        n->hash = hash;
        ::new (static_cast<void*>(std::addressof(n->value))) T(std::forward<Args>(args)...);
        return n;
    }

    void free_all() noexcept {
        if (ctl_ != nullptr) {
            if (ctl_->size != 0)
                detail::hash_destroy_all(ctl_, &destroy_node);
            ::free(ctl_);
            ctl_ = nullptr;
        }
    }

    // Ensure the block can take one more element (grow at the load-factor
    // threshold). After this, ctl_ is non-null.
    void reserve_for_one_more() {
        size_t bc = bucket_count();
        float mlf = max_load_factor();
        if (bc == 0 || float(size() + 1) > float(bc) * mlf) {
            size_t need = size_t(__builtin_ceilf(float(size() + 1) / mlf));
            size_t want = 2 * bc;
            if (need > want)
                want = need;
            ctl_ = detail::hash_set_bucket_count(ctl_, detail::hash_pow2_ceil(want));
        }
    }

    template<class K>
    base* find_node(size_t hash, const K& k) const {
        if (ctl_ == nullptr)
            return nullptr;
        size_t mask = ctl_->bucket_count - 1;
        size_t chash = hash & mask;
        base* pn = detail::hash_buckets(ctl_)[chash];
        if (pn != nullptr) {
            for (base* nd = pn->next; nd != nullptr && (nd->hash & mask) == chash; nd = nd->next)
                if (nd->hash == hash && eq_(node_value(nd), k))
                    return nd;
        }
        return nullptr;
    }

public:
    // ------------------------------------------------------------------
    // Construction / assignment
    // ------------------------------------------------------------------

    hash_table() noexcept(is_nothrow_default_constructible_v<Hash> &&
                          is_nothrow_default_constructible_v<Equal>) = default;

    hash_table(const hasher& hf, const key_equal& eq) : hasher_(hf), eq_(eq) {}
    hash_table(const hasher& hf, const key_equal& eq, const allocator_type&)
        : hasher_(hf), eq_(eq) {}
    explicit hash_table(const allocator_type&) {}

    // Copy: copy only the policy (hasher/equal) and, if the source owns a block,
    // an EMPTY block of the same shape (bucket_count + max_load_factor). The
    // owning container copies the elements afterward via insert.
    hash_table(const hash_table& u) : hasher_(u.hasher_), eq_(u.eq_) {
        if (u.ctl_ != nullptr) {
            ctl_ = detail::hash_set_bucket_count(nullptr, u.ctl_->bucket_count);
            ctl_->max_load_factor = u.ctl_->max_load_factor;
        }
    }
    hash_table(const hash_table& u, const allocator_type&) : hash_table(u) {}

    hash_table(hash_table&& u) noexcept(is_nothrow_move_constructible_v<Hash> &&
                                        is_nothrow_move_constructible_v<Equal>)
        : ctl_(u.ctl_), hasher_(std::move(u.hasher_)), eq_(std::move(u.eq_)) {
        u.ctl_ = nullptr;
    }
    hash_table(hash_table&& u, const allocator_type&)
        : ctl_(u.ctl_), hasher_(std::move(u.hasher_)), eq_(std::move(u.eq_)) {
        u.ctl_ = nullptr;
    }

    ~hash_table() { free_all(); }

    hash_table& operator=(const hash_table& u) {
        if (this != &u) {
            free_all();
            hasher_ = u.hasher_;
            eq_ = u.eq_;
            if (u.ctl_ != nullptr) {
                ctl_ = detail::hash_set_bucket_count(nullptr, u.ctl_->bucket_count);
                ctl_->max_load_factor = u.ctl_->max_load_factor;
                for (const_iterator i = u.begin(), e = u.end(); i != e; ++i)
                    insert_unique(*i);
            }
        }
        return *this;
    }

    hash_table& operator=(hash_table&& u) noexcept(is_nothrow_move_assignable_v<Hash> &&
                                                   is_nothrow_move_assignable_v<Equal>) {
        if (this != &u) {
            free_all();
            hasher_ = std::move(u.hasher_);
            eq_ = std::move(u.eq_);
            ctl_ = u.ctl_;
            u.ctl_ = nullptr;
        }
        return *this;
    }

    template<class It>
    void assign_unique(It first, It last) {
        clear();
        for (; first != last; ++first)
            insert_unique(*first);
    }

    allocator_type node_alloc() const noexcept { return allocator_type(); }

    // ------------------------------------------------------------------
    // Size / iterators
    // ------------------------------------------------------------------

    size_type size() const noexcept { return ctl_ != nullptr ? ctl_->size : 0; }
    size_type max_size() const noexcept {
        return numeric_limits<size_type>::max() / sizeof(node);
    }

    iterator begin() noexcept { return iterator(ctl_ != nullptr ? ctl_->first.next : nullptr); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept {
        return const_iterator(ctl_ != nullptr ? ctl_->first.next : nullptr);
    }
    const_iterator end() const noexcept { return const_iterator(nullptr); }

    hasher hash_function() const { return hasher_; }
    key_equal key_eq() const { return eq_; }

    // ------------------------------------------------------------------
    // Insert / emplace (unique keys)
    // ------------------------------------------------------------------

    template<class Key2, class... Args>
    pair<iterator, bool> emplace_unique_key_args(const Key2& k, Args&&... args) {
        size_t hash = hasher_(k);
        base* existing = find_node(hash, k);
        if (existing != nullptr)
            return pair<iterator, bool>(iterator(existing), false);
        node* nd = create_node(hash, std::forward<Args>(args)...);
        reserve_for_one_more();
        detail::hash_link_unique(ctl_, nd);
        return pair<iterator, bool>(iterator(nd), true);
    }

    template<class... Args>
    pair<iterator, bool> emplace_unique_impl(Args&&... args) {
        // Construct the node first (we must, to know its key), then look for a
        // duplicate; drop the node if one exists.
        node* nd = create_node(0, std::forward<Args>(args)...);
        nd->hash = hasher_(nd->value);
        base* existing = find_node(nd->hash, kv::get_key(nd->value));
        if (existing != nullptr) {
            destroy_node(nd);
            return pair<iterator, bool>(iterator(existing), false);
        }
        reserve_for_one_more();
        detail::hash_link_unique(ctl_, nd);
        return pair<iterator, bool>(iterator(nd), true);
    }

    template<class P>
    pair<iterator, bool> emplace_unique_extract(P&& x, detail::extract_key_fail_tag) {
        return emplace_unique_impl(std::forward<P>(x));
    }
    template<class P>
    pair<iterator, bool> emplace_unique_extract(P&& x, detail::extract_key_self_tag) {
        return emplace_unique_key_args(x, std::forward<P>(x));
    }
    template<class P>
    pair<iterator, bool> emplace_unique_extract(P&& x, detail::extract_key_first_tag) {
        return emplace_unique_key_args(x.first, std::forward<P>(x));
    }

    template<class P>
    pair<iterator, bool> emplace_unique(P&& x) {
        return emplace_unique_extract(std::forward<P>(x),
                                      detail::ht_can_extract_key<P, typename kv::key_type>());
    }

    template<class First, class Second,
             enable_if_t<detail::ht_can_extract_map_key<First, typename kv::key_type, T>::value,
                         int> = 0>
    pair<iterator, bool> emplace_unique(First&& f, Second&& s) {
        return emplace_unique_key_args(f, std::forward<First>(f), std::forward<Second>(s));
    }

    template<class... Args>
    pair<iterator, bool> emplace_unique(Args&&... args) {
        return emplace_unique_impl(std::forward<Args>(args)...);
    }

    pair<iterator, bool> insert_unique(const value_type& x) {
        return emplace_unique_key_args(kv::get_key(x), x);
    }
    pair<iterator, bool> insert_unique(value_type&& x) {
        return emplace_unique_key_args(kv::get_key(x), std::move(x));
    }
    template<class P, enable_if_t<!is_same_v<remove_cv_t<remove_reference_t<P>>, value_type>,
                                  int> = 0>
    pair<iterator, bool> insert_unique(P&& x) {
        return emplace_unique(std::forward<P>(x));
    }

    // ------------------------------------------------------------------
    // Erase
    // ------------------------------------------------------------------

    iterator erase(const_iterator p) noexcept {
        base* cn = p.node_;
        base* nx = cn->next;
        detail::hash_unlink(ctl_, cn);
        destroy_node(cn);
        return iterator(nx);
    }

    iterator erase(const_iterator first, const_iterator last) noexcept {
        while (first != last) {
            const_iterator p = first;
            ++first;
            erase(p);
        }
        return iterator(last.node_);
    }

    template<class K>
    size_type erase_unique(const K& k) {
        iterator i = find(k);
        if (i == end())
            return 0;
        erase(i);
        return 1;
    }

    void clear() noexcept {
        if (ctl_ != nullptr && ctl_->size != 0)
            detail::hash_destroy_all(ctl_, &destroy_node);
    }

    // ------------------------------------------------------------------
    // Lookup
    // ------------------------------------------------------------------

    template<class K>
    iterator find(const K& k) {
        return iterator(find_node(hasher_(k), k));
    }
    template<class K>
    const_iterator find(const K& k) const {
        return const_iterator(find_node(hasher_(k), k));
    }

    template<class K>
    size_type count_unique(const K& k) const {
        return find(k) != end() ? 1 : 0;
    }

    template<class K>
    pair<iterator, iterator> equal_range_unique(const K& k) {
        iterator i = find(k);
        iterator j = i;
        if (i != end())
            ++j;
        return pair<iterator, iterator>(i, j);
    }
    template<class K>
    pair<const_iterator, const_iterator> equal_range_unique(const K& k) const {
        const_iterator i = find(k);
        const_iterator j = i;
        if (i != end())
            ++j;
        return pair<const_iterator, const_iterator>(i, j);
    }

    // ------------------------------------------------------------------
    // Hash policy (reduced local-bucket API per the lean contract)
    // ------------------------------------------------------------------

    size_type bucket_count() const noexcept { return ctl_ != nullptr ? ctl_->bucket_count : 0; }
    size_type max_bucket_count() const noexcept { return max_size(); }

    float load_factor() const noexcept {
        size_type bc = bucket_count();
        return bc != 0 ? float(size()) / float(bc) : 0.0f;
    }
    float max_load_factor() const noexcept {
        return ctl_ != nullptr ? ctl_->max_load_factor : 1.0f;
    }
    void max_load_factor(float mlf) {
        float lf = load_factor();
        float newmlf = mlf > lf ? mlf : lf;
        if (ctl_ == nullptr)
            ctl_ = detail::hash_set_bucket_count(nullptr, 2); // minimal block to hold mlf
        ctl_->max_load_factor = newmlf;
    }

    void rehash_unique(size_type n) {
        size_t minb = size_t(__builtin_ceilf(float(size()) / max_load_factor()));
        if (n < minb)
            n = minb;
        if (n == 0)
            return;
        size_t nbc = detail::hash_pow2_ceil(n);
        if (ctl_ != nullptr && nbc == ctl_->bucket_count)
            return;
        ctl_ = detail::hash_set_bucket_count(ctl_, nbc);
    }
    void reserve_unique(size_type n) {
        rehash_unique(size_type(__builtin_ceilf(float(n) / max_load_factor())));
    }

    // ------------------------------------------------------------------
    // Swap (O(1): the block owns all state, so swap the pointers)
    // ------------------------------------------------------------------

    void swap(hash_table& u) noexcept(is_nothrow_swappable_v<Hash> &&
                                      is_nothrow_swappable_v<Equal>) {
        using std::swap;
        swap(ctl_, u.ctl_);
        swap(hasher_, u.hasher_);
        swap(eq_, u.eq_);
    }
};

template<class T, class Hash, class Equal, class Alloc>
inline void swap(hash_table<T, Hash, Equal, Alloc>& x, hash_table<T, Hash, Equal, Alloc>& y)
    noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

} // namespace std
