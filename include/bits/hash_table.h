// bits/hash_table.h — internal hash table infrastructure for the unordered
// containers. Ported from libc++ include/__hash_table (release/20.x), plus:
//   - __next_prime from libcxx/src/hash.cpp (header-only here),
//   - node-handle machinery from include/__node_handle,
//   - key-extraction tags from include/__type_traits/can_extract_key.h.
// Shared by <unordered_map> and <unordered_set>.
//
// Deviations from libc++ (reasons in line):
//   - libc++ stores map elements as __hash_value_type<Key,T>, a layout-
//     identical wrapper around pair<const Key, T> whose only purpose is to
//     allow assigning/moving nodes despite the const key. Here the node value
//     type IS pair<const Key, T> and the const_cast tricks live in
//     ht_kv_types (same object layout, same casts, fewer types). As a result
//     the map needs no __hash_map_iterator wrappers: the plain hash iterators
//     already yield pair<const Key, T>.
//   - The bucket list is managed with explicit allocate/deallocate calls
//     instead of unique_ptr<next_ptr[], deleter> (same allocator, same calls).
//   - would-throw paths trap (house style, -fno-exceptions).
//
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <optional>
#include <limits>
#include <cstddef>

namespace std {

namespace detail {

// ===========================================================================
// __next_prime — ported from libcxx/src/hash.cpp
// ===========================================================================

// handle all next_prime(i) for i in [1, 210), special case 0
inline constexpr unsigned ht_small_primes[] = {
    0,   2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113, 127,
    131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211};

// potential primes = 210*k + ht_prime_indices[i], k >= 1
//   these numbers are not divisible by 2, 3, 5 or 7.
inline constexpr unsigned ht_prime_indices[] = {
    1,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,  53,  59,  61,  67,
    71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113, 121, 127, 131, 137, 139,
    143, 149, 151, 157, 163, 167, 169, 173, 179, 181, 187, 191, 193, 197, 199, 209};

template<class T>
inline const T* ht_lower_bound(const T* first, const T* last, size_t value) {
    size_t len = static_cast<size_t>(last - first);
    while (len != 0) {
        size_t half = len / 2;
        const T* mid = first + half;
        if (static_cast<size_t>(*mid) < value) {
            first = mid + 1;
            len -= half + 1;
        } else
            len = half;
    }
    return first;
}

// If n == 0, returns 0. Else returns the lowest prime number >= n.
inline size_t next_prime(size_t n) {
    const size_t L = 210;
    const size_t N = sizeof(ht_small_primes) / sizeof(ht_small_primes[0]);
    // If n is small enough, search in small_primes
    if (n <= ht_small_primes[N - 1])
        return *ht_lower_bound(ht_small_primes, ht_small_primes + N, n);
    // Else n > largest small prime. Check for overflow (would throw
    // overflow_error in libc++).
    if (sizeof(size_t) == 8 ? n > 0xFFFFFFFFFFFFFFC5ull : n > 0xFFFFFFFBu)
        __builtin_trap();
    const size_t M = sizeof(ht_prime_indices) / sizeof(ht_prime_indices[0]);
    size_t k0 = n / L;
    size_t in = static_cast<size_t>(
        ht_lower_bound(ht_prime_indices, ht_prime_indices + M, n - k0 * L) - ht_prime_indices);
    n = L * k0 + ht_prime_indices[in];
    while (true) {
        // Divide n by all primes or potential primes (i) until:
        //   1. The division is even, so try next potential prime.
        //   2. The i > sqrt(n), in which case n is prime.
        for (size_t j = 5; j < N - 1; ++j) {
            const size_t p = ht_small_primes[j];
            const size_t q = n / p;
            if (q < p)
                return n;
            if (n == q * p)
                goto next;
        }
        // n wasn't divisible by small primes, try potential primes
        {
            size_t i = 211;
            while (true) {
                size_t q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 10; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 8; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 8; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 6; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 4; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 10; q = n / i;
                if (q < i) return n;
                if (n == q * i) break;
                i += 2;
            }
        }
    next:
        // n is not prime. Increment n to next potential prime.
        if (++in == M) {
            ++k0;
            in = 0;
        }
        n = L * k0 + ht_prime_indices[in];
    }
}

// ===========================================================================
// Hash constraining helpers (libc++ __hash_table)
// ===========================================================================

inline bool is_hash_power2(size_t bc) noexcept {
    return bc > 2 && !(bc & (bc - 1));
}

inline size_t constrain_hash(size_t h, size_t bc) noexcept {
    return !(bc & (bc - 1)) ? h & (bc - 1) : (h < bc ? h : h % bc);
}

inline size_t next_hash_pow2(size_t n) noexcept {
    return n < 2 ? n
                 : (size_t(1) << (numeric_limits<size_t>::digits - __builtin_clzll(n - 1)));
}

inline float ht_ceil(float f) noexcept { return __builtin_ceilf(f); }

// ===========================================================================
// Key extraction tags (libc++ __type_traits/can_extract_key.h)
// ===========================================================================

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

// true iff Key != ContainerValueTy (map, not set) and ValTy == Key.
template<class ValTy, class Key, class ContainerValueTy,
         class RawValTy = remove_const_t<remove_reference_t<ValTy>>>
struct ht_can_extract_map_key : bool_constant<is_same_v<RawValTy, Key>> {};

template<class ValTy, class Key, class RawValTy>
struct ht_can_extract_map_key<ValTy, Key, Key, RawValTy> : false_type {};

// is_transparent gate for the C++20 heterogeneous overloads
// (libc++ __is_transparent_v<Hash,K> && __is_transparent_v<Pred,K>)
template<class T, class K, class = void>
inline constexpr bool ht_is_transparent_v = false;
template<class T, class K>
inline constexpr bool ht_is_transparent_v<T, K, void_t<typename T::is_transparent>> = true;

// __is_allocator (unique name; detail::tree_is_allocator_v is owned by
// bits/tree.h)
template<class Alloc, class = void, class = void>
inline constexpr bool ht_is_allocator_v = false;
template<class Alloc>
inline constexpr bool ht_is_allocator_v<
    Alloc, void_t<typename Alloc::value_type>,
    void_t<decltype(declval<Alloc&>().allocate(size_t(0)))>> = true;

template<class Iter>
using ht_iter_category_t = typename iterator_traits<Iter>::iterator_category;

// ===========================================================================
// Key/value traits (libc++ __hash_key_value_types, adapted: the map node
// value type is pair<const Key, T> itself; the const_cast machinery of
// __hash_value_type lives here instead)
// ===========================================================================

template<class Tp>
struct ht_kv_types {
    using key_type             = Tp;
    using node_value_type      = Tp;
    using container_value_type = Tp;
    static const bool is_map   = false;

    static const key_type& get_key(const Tp& v) noexcept { return v; }
    static container_value_type* get_ptr(node_value_type& n) noexcept { return std::addressof(n); }
    static Tp&& move_value(node_value_type& v) noexcept { return std::move(v); }
    static Tp& ref(node_value_type& v) noexcept { return v; }
};

template<class Key, class T>
struct ht_kv_types<pair<const Key, T>> {
    using key_type             = Key;
    using mapped_type          = T;
    using node_value_type      = pair<const Key, T>;
    using container_value_type = pair<const Key, T>;
    static const bool is_map   = true;

    static const key_type& get_key(const container_value_type& v) noexcept { return v.first; }
    static container_value_type* get_ptr(node_value_type& n) noexcept { return std::addressof(n); }
    // __hash_value_type::__move()
    static pair<Key&&, T&&> move_value(node_value_type& v) noexcept {
        return pair<Key&&, T&&>(std::move(const_cast<Key&>(v.first)), std::move(v.second));
    }
    // __hash_value_type::__ref()
    static pair<Key&, T&> ref(node_value_type& v) noexcept {
        return pair<Key&, T&>(const_cast<Key&>(v.first), v.second);
    }
};

} // namespace detail

// ===========================================================================
// Node types (libc++ __hash_node_base / __hash_node)
// ===========================================================================

template<class Tp, class Hash, class Equal, class Alloc>
class hash_table;

template<class NodePtr>
struct ht_node_base {
    using node_type         = typename pointer_traits<NodePtr>::element_type;
    using node_base_pointer = typename pointer_traits<NodePtr>::template rebind<ht_node_base>;
    using node_pointer      = NodePtr;
    using next_pointer      = node_base_pointer;

    next_pointer next_;

    next_pointer ptr() noexcept {
        return static_cast<next_pointer>(pointer_traits<node_base_pointer>::pointer_to(*this));
    }

    node_pointer upcast() noexcept {
        return static_cast<node_pointer>(pointer_traits<node_base_pointer>::pointer_to(*this));
    }

    size_t hash() const noexcept { return static_cast<const node_type&>(*this).hash_; }

    ht_node_base() noexcept : next_(nullptr) {}
    explicit ht_node_base(next_pointer next) noexcept : next_(next) {}
};

template<class Tp, class VoidPtr>
struct ht_node
    : public ht_node_base<typename pointer_traits<VoidPtr>::template rebind<ht_node<Tp, VoidPtr>>>
{
    using node_value_type = Tp;
    using base            = ht_node_base<typename pointer_traits<VoidPtr>::template rebind<ht_node>>;
    using next_pointer    = typename base::next_pointer;

    size_t hash_;

    // Lifetime of the value starts separately (allocator-aware construction).
private:
    union { Tp value_; };
public:
    Tp& get_value() noexcept { return value_; }
    const Tp& get_value() const noexcept { return value_; }

    explicit ht_node(next_pointer next, size_t hash) : base(next), hash_(hash) {}
    ~ht_node() {}
};

// ht_node_types: compute iterator typedefs from NodePtr WITHOUT completing
// the node type (libc++ __hash_node_types). Required for incomplete-type
// support: `struct A { unordered_set<A> m; ... }` instantiates the iterator
// classes while A is incomplete, and `typename node_type::next_pointer`
// would otherwise force completion of ht_node<A, ...> (whose union member
// needs complete A).
template<class Node>
struct ht_node_types_impl;

template<class T, class VP>
struct ht_node_types_impl<ht_node<T, VP>> {
    using node_value_type   = T;
    using node_pointer      = typename pointer_traits<VP>::template rebind<ht_node<T, VP>>;
    using node_base_pointer =
        typename pointer_traits<VP>::template rebind<ht_node_base<node_pointer>>;
    using next_pointer      = node_base_pointer;
};

template<class NodePtr>
using ht_node_types = ht_node_types_impl<typename pointer_traits<NodePtr>::element_type>;

// ===========================================================================
// Iterators (libc++ __hash_iterator and friends)
// Members are public implementation details (libcis style avoids the long
// cross-class friend lists of libc++; see CONVENTIONS gcc-10 note (f)).
// ===========================================================================

template<class NodePtr>
class ht_iterator {
public:
    using node_type    = typename pointer_traits<NodePtr>::element_type;
    using next_pointer = typename ht_node_types<NodePtr>::next_pointer;

    next_pointer node_;

    using iterator_category = forward_iterator_tag;
    using value_type        = typename ht_node_types<NodePtr>::node_value_type;
    using difference_type   = ptrdiff_t;
    using reference         = value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<value_type>;

    ht_iterator() noexcept : node_(nullptr) {}
    explicit ht_iterator(next_pointer node) noexcept : node_(node) {}

    reference operator*() const { return node_->upcast()->get_value(); }
    pointer operator->() const {
        return pointer_traits<pointer>::pointer_to(node_->upcast()->get_value());
    }
    ht_iterator& operator++() { node_ = node_->next_; return *this; }
    ht_iterator operator++(int) { ht_iterator t(*this); ++(*this); return t; }

    friend bool operator==(const ht_iterator& x, const ht_iterator& y) { return x.node_ == y.node_; }
    friend bool operator!=(const ht_iterator& x, const ht_iterator& y) { return !(x == y); }
};

template<class NodePtr>
class ht_const_iterator {
public:
    using node_type    = typename pointer_traits<NodePtr>::element_type;
    using next_pointer = typename ht_node_types<NodePtr>::next_pointer;

    next_pointer node_;

    using non_const_iterator = ht_iterator<NodePtr>;

    using iterator_category = forward_iterator_tag;
    using value_type        = typename ht_node_types<NodePtr>::node_value_type;
    using difference_type   = ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<const value_type>;

    ht_const_iterator() noexcept : node_(nullptr) {}
    explicit ht_const_iterator(next_pointer node) noexcept : node_(node) {}
    ht_const_iterator(const non_const_iterator& x) noexcept : node_(x.node_) {}

    reference operator*() const { return node_->upcast()->get_value(); }
    pointer operator->() const {
        return pointer_traits<pointer>::pointer_to(node_->upcast()->get_value());
    }
    ht_const_iterator& operator++() { node_ = node_->next_; return *this; }
    ht_const_iterator operator++(int) { ht_const_iterator t(*this); ++(*this); return t; }

    friend bool operator==(const ht_const_iterator& x, const ht_const_iterator& y) { return x.node_ == y.node_; }
    friend bool operator!=(const ht_const_iterator& x, const ht_const_iterator& y) { return !(x == y); }
};

template<class NodePtr>
class ht_local_iterator {
public:
    using node_type    = typename pointer_traits<NodePtr>::element_type;
    using next_pointer = typename ht_node_types<NodePtr>::next_pointer;

    next_pointer node_;
    size_t bucket_;
    size_t bucket_count_;

    using iterator_category = forward_iterator_tag;
    using value_type        = typename ht_node_types<NodePtr>::node_value_type;
    using difference_type   = ptrdiff_t;
    using reference         = value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<value_type>;

    ht_local_iterator() noexcept : node_(nullptr), bucket_(0), bucket_count_(0) {}
    // node is the bucket's "before begin" pointer (or null)
    explicit ht_local_iterator(next_pointer node, size_t bucket, size_t bucket_count) noexcept
        : node_(node), bucket_(bucket), bucket_count_(bucket_count) {
        if (node_ != nullptr)
            node_ = node_->next_;
    }

    reference operator*() const { return node_->upcast()->get_value(); }
    pointer operator->() const {
        return pointer_traits<pointer>::pointer_to(node_->upcast()->get_value());
    }
    ht_local_iterator& operator++() {
        node_ = node_->next_;
        if (node_ != nullptr && detail::constrain_hash(node_->hash(), bucket_count_) != bucket_)
            node_ = nullptr;
        return *this;
    }
    ht_local_iterator operator++(int) { ht_local_iterator t(*this); ++(*this); return t; }

    friend bool operator==(const ht_local_iterator& x, const ht_local_iterator& y) { return x.node_ == y.node_; }
    friend bool operator!=(const ht_local_iterator& x, const ht_local_iterator& y) { return !(x == y); }
};

template<class NodePtr>
class ht_const_local_iterator {
public:
    using node_type    = typename pointer_traits<NodePtr>::element_type;
    using next_pointer = typename ht_node_types<NodePtr>::next_pointer;

    next_pointer node_;
    size_t bucket_;
    size_t bucket_count_;

    using non_const_iterator = ht_local_iterator<NodePtr>;

    using iterator_category = forward_iterator_tag;
    using value_type        = typename ht_node_types<NodePtr>::node_value_type;
    using difference_type   = ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<const value_type>;

    ht_const_local_iterator() noexcept : node_(nullptr), bucket_(0), bucket_count_(0) {}
    explicit ht_const_local_iterator(next_pointer node, size_t bucket, size_t bucket_count) noexcept
        : node_(node), bucket_(bucket), bucket_count_(bucket_count) {
        if (node_ != nullptr)
            node_ = node_->next_;
    }
    ht_const_local_iterator(const non_const_iterator& x) noexcept
        : node_(x.node_), bucket_(x.bucket_), bucket_count_(x.bucket_count_) {}

    reference operator*() const { return node_->upcast()->get_value(); }
    pointer operator->() const {
        return pointer_traits<pointer>::pointer_to(node_->upcast()->get_value());
    }
    ht_const_local_iterator& operator++() {
        node_ = node_->next_;
        if (node_ != nullptr && detail::constrain_hash(node_->hash(), bucket_count_) != bucket_)
            node_ = nullptr;
        return *this;
    }
    ht_const_local_iterator operator++(int) { ht_const_local_iterator t(*this); ++(*this); return t; }

    friend bool operator==(const ht_const_local_iterator& x, const ht_const_local_iterator& y) { return x.node_ == y.node_; }
    friend bool operator!=(const ht_const_local_iterator& x, const ht_const_local_iterator& y) { return !(x == y); }
};

// ===========================================================================
// Node destructor (libc++ __hash_node_destructor)
// ===========================================================================

template<class Alloc>
class ht_node_destructor {
    using allocator_type = Alloc;
    using alloc_traits   = allocator_traits<allocator_type>;

public:
    using pointer = typename alloc_traits::pointer;

private:
    using node_type = typename pointer_traits<pointer>::element_type;
    using kv_types  = detail::ht_kv_types<typename node_type::node_value_type>;

    allocator_type& na_;

public:
    bool value_constructed;

    ht_node_destructor(const ht_node_destructor&) = default;
    ht_node_destructor& operator=(const ht_node_destructor&) = delete;

    explicit ht_node_destructor(allocator_type& na, bool constructed = false) noexcept
        : na_(na), value_constructed(constructed) {}

    void operator()(pointer p) noexcept {
        if (value_constructed) {
            alloc_traits::destroy(na_, kv_types::get_ptr(p->get_value()));
            std::destroy_at(std::addressof(*p));
        }
        if (p)
            alloc_traits::deallocate(na_, p, 1);
    }
};

// ===========================================================================
// Node handles (libc++ __node_handle: __basic_node_handle + specifics)
// ===========================================================================

template<class NodeType, class Alloc, template<class, class> class MapOrSetSpecifics>
class ht_basic_node_handle
    : public MapOrSetSpecifics<NodeType, ht_basic_node_handle<NodeType, Alloc, MapOrSetSpecifics>>
{
    using alloc_traits = allocator_traits<Alloc>;

public:
    using allocator_type = Alloc;
    using node_pointer_type =
        typename pointer_traits<typename alloc_traits::void_pointer>::template rebind<NodeType>;

    // public implementation detail (avoids cross-class friend issues, gcc-10 note (f))
    node_pointer_type ptr_ = nullptr;
    optional<allocator_type> alloc_;

    void release_ptr() noexcept {
        ptr_ = nullptr;
        alloc_ = nullopt;
    }

    void destroy_node_pointer() noexcept {
        if (ptr_ != nullptr) {
            using node_alloc_type = typename alloc_traits::template rebind_alloc<NodeType>;
            node_alloc_type alloc(*alloc_);
            ht_node_destructor<node_alloc_type>(alloc, true)(ptr_);
            ptr_ = nullptr;
        }
    }

    ht_basic_node_handle(node_pointer_type p, const allocator_type& a) : ptr_(p), alloc_(a) {}

    ht_basic_node_handle() = default;

    ht_basic_node_handle(ht_basic_node_handle&& other) noexcept
        : ptr_(other.ptr_), alloc_(std::move(other.alloc_)) {
        other.ptr_ = nullptr;
        other.alloc_ = nullopt;
    }

    ht_basic_node_handle& operator=(ht_basic_node_handle&& other) {
        destroy_node_pointer();
        ptr_ = other.ptr_;
        if (alloc_traits::propagate_on_container_move_assignment::value || alloc_ == nullopt)
            alloc_ = std::move(other.alloc_);
        other.ptr_ = nullptr;
        other.alloc_ = nullopt;
        return *this;
    }

    allocator_type get_allocator() const { return *alloc_; }

    explicit operator bool() const { return ptr_ != nullptr; }

    [[nodiscard]] bool empty() const { return ptr_ == nullptr; }

    void swap(ht_basic_node_handle& other) noexcept(
        alloc_traits::propagate_on_container_swap::value || alloc_traits::is_always_equal::value) {
        using std::swap;
        swap(ptr_, other.ptr_);
        if (alloc_traits::propagate_on_container_swap::value || alloc_ == nullopt ||
            other.alloc_ == nullopt)
            swap(alloc_, other.alloc_);
    }

    friend void swap(ht_basic_node_handle& a, ht_basic_node_handle& b) noexcept(noexcept(a.swap(b))) {
        a.swap(b);
    }

    ~ht_basic_node_handle() { destroy_node_pointer(); }
};

template<class NodeType, class Derived>
struct ht_set_node_handle_specifics {
    using value_type = typename NodeType::node_value_type;

    value_type& value() const { return static_cast<const Derived*>(this)->ptr_->get_value(); }
};

template<class NodeType, class Derived>
struct ht_map_node_handle_specifics {
    using kv_types    = detail::ht_kv_types<typename NodeType::node_value_type>;
    using key_type    = typename kv_types::key_type;
    using mapped_type = typename kv_types::mapped_type;

    key_type& key() const {
        return kv_types::ref(static_cast<const Derived*>(this)->ptr_->get_value()).first;
    }
    mapped_type& mapped() const {
        return kv_types::ref(static_cast<const Derived*>(this)->ptr_->get_value()).second;
    }
};

template<class NodeType, class Alloc>
using ht_set_node_handle = ht_basic_node_handle<NodeType, Alloc, ht_set_node_handle_specifics>;

template<class NodeType, class Alloc>
using ht_map_node_handle = ht_basic_node_handle<NodeType, Alloc, ht_map_node_handle_specifics>;

template<class Iterator, class NodeType>
struct ht_insert_return_type {
    Iterator position;
    bool inserted;
    NodeType node;
};

// ===========================================================================
// hash_table (libc++ __hash_table)
// ===========================================================================

template<class Tp, class Hash, class Equal, class Alloc>
class hash_table {
public:
    using value_type     = Tp;
    using hasher         = Hash;
    using key_equal      = Equal;
    using allocator_type = Alloc;

    using alloc_traits = allocator_traits<allocator_type>;
    using kv_types     = detail::ht_kv_types<Tp>;

    using node_value_type      = typename kv_types::node_value_type;
    using container_value_type = typename kv_types::container_value_type;
    using key_type             = typename kv_types::key_type;
    using reference            = value_type&;
    using const_reference      = const value_type&;
    using pointer              = typename alloc_traits::pointer;
    using const_pointer        = typename alloc_traits::const_pointer;
    using size_type            = typename alloc_traits::size_type;
    using difference_type      = ptrdiff_t;

    using void_pointer = typename alloc_traits::void_pointer;
    using node         = ht_node<Tp, void_pointer>;
    using node_allocator = typename alloc_traits::template rebind_alloc<node>;
    using node_traits    = allocator_traits<node_allocator>;
    using node_pointer   = typename node_traits::pointer;
    using first_node_t   = ht_node_base<node_pointer>;
    using node_base_pointer = typename first_node_t::node_base_pointer;
    using next_pointer      = typename first_node_t::next_pointer;

    using pointer_allocator   = typename node_traits::template rebind_alloc<next_pointer>;
    using pointer_alloc_traits = allocator_traits<pointer_allocator>;
    using bucket_list_pointer = typename pointer_alloc_traits::pointer;

    using iterator             = ht_iterator<node_pointer>;
    using const_iterator       = ht_const_iterator<node_pointer>;
    using local_iterator       = ht_local_iterator<node_pointer>;
    using const_local_iterator = ht_const_local_iterator<node_pointer>;

    using node_destructor = ht_node_destructor<node_allocator>;
    using node_holder     = unique_ptr<node, node_destructor>;

private:
    // --- Member data (libc++ layout: bucket list, first node, node alloc,
    // size, hasher, max load factor, key_eq; [[no_unique_address]] members
    // first per gcc-10 note (a)) ---
    [[no_unique_address]] node_allocator node_alloc_;
    [[no_unique_address]] hasher hasher_;
    [[no_unique_address]] key_equal key_eq_;
    bucket_list_pointer bucket_list_;
    size_type bucket_count_;
    first_node_t first_node_;
    size_type size_;
    float max_load_factor_;

public:
    size_type size() const noexcept { return size_; }

    hasher& hash_function() noexcept { return hasher_; }
    const hasher& hash_function() const noexcept { return hasher_; }

    float& max_load_factor_ref() noexcept { return max_load_factor_; }
    float max_load_factor() const noexcept { return max_load_factor_; }

    key_equal& key_eq() noexcept { return key_eq_; }
    const key_equal& key_eq() const noexcept { return key_eq_; }

    node_allocator& node_alloc() noexcept { return node_alloc_; }
    const node_allocator& node_alloc() const noexcept { return node_alloc_; }

    size_type bucket_count() const noexcept { return bucket_count_; }

private:
    size_type& size_ref() noexcept { return size_; }

    void allocate_bucket_list(size_type n) {
        pointer_allocator pa(node_alloc_);
        bucket_list_ = n > 0 ? pointer_alloc_traits::allocate(pa, n) : nullptr;
        bucket_count_ = n;
    }

    void deallocate_bucket_list() noexcept {
        if (bucket_list_ != nullptr) {
            pointer_allocator pa(node_alloc_);
            pointer_alloc_traits::deallocate(pa, bucket_list_, bucket_count_);
            bucket_list_ = nullptr;
            bucket_count_ = 0;
        }
    }

public:
    // ------------------------------------------------------------------
    // Constructors / destructor
    // ------------------------------------------------------------------

    hash_table() noexcept(
        is_nothrow_default_constructible_v<node_allocator> &&
        is_nothrow_default_constructible_v<hasher> && is_nothrow_default_constructible_v<key_equal>)
        : node_alloc_(), hasher_(), key_eq_(),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0), max_load_factor_(1.0f) {}

    hash_table(const hasher& hf, const key_equal& eql)
        : node_alloc_(), hasher_(hf), key_eq_(eql),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0), max_load_factor_(1.0f) {}

    hash_table(const hasher& hf, const key_equal& eql, const allocator_type& a)
        : node_alloc_(node_allocator(a)), hasher_(hf), key_eq_(eql),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0), max_load_factor_(1.0f) {}

    explicit hash_table(const allocator_type& a)
        : node_alloc_(node_allocator(a)), hasher_(), key_eq_(),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0), max_load_factor_(1.0f) {}

    // Note: like libc++, the copy constructor copies hash/eq/mlf and the
    // (select_on_container_copy_construction) allocator but NO elements;
    // the containers re-insert.
    hash_table(const hash_table& u)
        : node_alloc_(node_traits::select_on_container_copy_construction(u.node_alloc())),
          hasher_(u.hasher_), key_eq_(u.key_eq_),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0),
          max_load_factor_(u.max_load_factor_) {}

    hash_table(const hash_table& u, const allocator_type& a)
        : node_alloc_(node_allocator(a)), hasher_(u.hasher_), key_eq_(u.key_eq_),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0),
          max_load_factor_(u.max_load_factor_) {}

    hash_table(hash_table&& u) noexcept(
        is_nothrow_move_constructible_v<node_allocator> &&
        is_nothrow_move_constructible_v<hasher> && is_nothrow_move_constructible_v<key_equal>)
        : node_alloc_(std::move(u.node_alloc_)), hasher_(std::move(u.hasher_)),
          key_eq_(std::move(u.key_eq_)),
          bucket_list_(u.bucket_list_), bucket_count_(u.bucket_count_),
          first_node_(std::move(u.first_node_)), size_(u.size_),
          max_load_factor_(u.max_load_factor_) {
        u.bucket_list_ = nullptr;
        u.bucket_count_ = 0;
        if (size() > 0) {
            bucket_list_[detail::constrain_hash(first_node_.next_->hash(), bucket_count())] =
                first_node_.ptr();
            u.first_node_.next_ = nullptr;
            u.size_ = 0;
        }
    }

    hash_table(hash_table&& u, const allocator_type& a)
        : node_alloc_(node_allocator(a)), hasher_(std::move(u.hasher_)),
          key_eq_(std::move(u.key_eq_)),
          bucket_list_(nullptr), bucket_count_(0), first_node_(), size_(0),
          max_load_factor_(u.max_load_factor_) {
        if (a == allocator_type(u.node_alloc())) {
            bucket_list_ = u.bucket_list_;
            bucket_count_ = u.bucket_count_;
            u.bucket_list_ = nullptr;
            u.bucket_count_ = 0;
            if (u.size() > 0) {
                first_node_.next_ = u.first_node_.next_;
                u.first_node_.next_ = nullptr;
                bucket_list_[detail::constrain_hash(first_node_.next_->hash(), bucket_count())] =
                    first_node_.ptr();
                size_ = u.size_;
                u.size_ = 0;
            }
        }
    }

    ~hash_table() {
        deallocate_node(first_node_.next_);
        deallocate_bucket_list();
    }

    // ------------------------------------------------------------------
    // Assignment
    // ------------------------------------------------------------------

private:
    void copy_assign_alloc(const hash_table& u) {
        if constexpr (node_traits::propagate_on_container_copy_assignment::value) {
            if (node_alloc() != u.node_alloc()) {
                clear();
                deallocate_bucket_list();
            }
            node_alloc_ = u.node_alloc_;
        }
    }

    void move_assign_alloc(hash_table& u) noexcept(
        !node_traits::propagate_on_container_move_assignment::value ||
        is_nothrow_move_assignable_v<node_allocator>) {
        if constexpr (node_traits::propagate_on_container_move_assignment::value)
            node_alloc_ = std::move(u.node_alloc_);
    }

    void move_assign(hash_table& u, true_type) noexcept(
        is_nothrow_move_assignable_v<node_allocator> && is_nothrow_move_assignable_v<hasher> &&
        is_nothrow_move_assignable_v<key_equal>) {
        clear();
        deallocate_bucket_list();
        bucket_list_ = u.bucket_list_;
        bucket_count_ = u.bucket_count_;
        u.bucket_list_ = nullptr;
        u.bucket_count_ = 0;
        move_assign_alloc(u);
        size_ = u.size_;
        hasher_ = std::move(u.hasher_);
        max_load_factor_ = u.max_load_factor_;
        key_eq_ = std::move(u.key_eq_);
        first_node_.next_ = u.first_node_.next_;
        if (size() > 0) {
            bucket_list_[detail::constrain_hash(first_node_.next_->hash(), bucket_count())] =
                first_node_.ptr();
            u.first_node_.next_ = nullptr;
            u.size_ = 0;
        }
    }

    void move_assign(hash_table& u, false_type) {
        if (node_alloc() == u.node_alloc())
            move_assign(u, true_type());
        else {
            hasher_ = std::move(u.hasher_);
            key_eq_ = std::move(u.key_eq_);
            max_load_factor_ = u.max_load_factor_;
            if (bucket_count() != 0) {
                next_pointer cache = detach();
                const_iterator i = u.begin();
                while (cache != nullptr && u.size() != 0) {
                    kv_types::ref(cache->upcast()->get_value()) =
                        kv_types::move_value(u.remove(i++)->get_value());
                    next_pointer next = cache->next_;
                    node_insert_multi(cache->upcast());
                    cache = next;
                }
                deallocate_node(cache);
            }
            const_iterator i = u.begin();
            while (u.size() != 0) {
                node_holder h = construct_node(kv_types::move_value(u.remove(i++)->get_value()));
                node_insert_multi(h.get());
                h.release();
            }
        }
    }

public:
    hash_table& operator=(const hash_table& u) {
        if (this != std::addressof(u)) {
            copy_assign_alloc(u);
            hasher_ = u.hasher_;
            key_eq_ = u.key_eq_;
            max_load_factor_ = u.max_load_factor_;
            assign_multi(u.begin(), u.end());
        }
        return *this;
    }

    hash_table& operator=(hash_table&& u) noexcept(
        node_traits::propagate_on_container_move_assignment::value &&
        is_nothrow_move_assignable_v<node_allocator> && is_nothrow_move_assignable_v<hasher> &&
        is_nothrow_move_assignable_v<key_equal>) {
        move_assign(u, integral_constant<bool,
                       node_traits::propagate_on_container_move_assignment::value>());
        return *this;
    }

    // ------------------------------------------------------------------
    // assign_unique / assign_multi (reuse existing nodes via cache)
    // ------------------------------------------------------------------

    template<class InputIterator>
    void assign_unique(InputIterator first, InputIterator last) {
        if (bucket_count() != 0) {
            next_pointer cache = detach();
            for (; cache != nullptr && first != last; ++first) {
                kv_types::ref(cache->upcast()->get_value()) = *first;
                next_pointer next = cache->next_;
                node_insert_unique(cache->upcast());
                cache = next;
            }
            deallocate_node(cache);
        }
        for (; first != last; ++first)
            insert_unique(*first);
    }

    template<class InputIterator>
    void assign_multi(InputIterator first, InputIterator last) {
        if (bucket_count() != 0) {
            next_pointer cache = detach();
            for (; cache != nullptr && first != last; ++first) {
                kv_types::ref(cache->upcast()->get_value()) = *first;
                next_pointer next = cache->next_;
                node_insert_multi(cache->upcast());
                cache = next;
            }
            deallocate_node(cache);
        }
        for (; first != last; ++first)
            insert_multi(*first);
    }

    size_type max_size() const noexcept {
        return std::min<size_type>(node_traits::max_size(node_alloc()),
                                   numeric_limits<difference_type>::max());
    }

private:
    void deallocate_node(next_pointer np) noexcept {
        node_allocator& na = node_alloc();
        while (np != nullptr) {
            next_pointer next = np->next_;
            node_pointer real_np = np->upcast();
            node_traits::destroy(na, kv_types::get_ptr(real_np->get_value()));
            std::destroy_at(std::addressof(*real_np));
            node_traits::deallocate(na, real_np, 1);
            np = next;
        }
    }

    next_pointer detach() noexcept {
        size_type bc = bucket_count();
        for (size_type i = 0; i < bc; ++i)
            bucket_list_[i] = nullptr;
        size_ = 0;
        next_pointer cache = first_node_.next_;
        first_node_.next_ = nullptr;
        return cache;
    }

    // Returns existing node if the value is already present, else null
    // (after rehashing if needed). Ported exactly.
    next_pointer node_insert_unique_prepare(size_t hash, value_type& value) {
        size_type bc = bucket_count();
        if (bc != 0) {
            size_t chash = detail::constrain_hash(hash, bc);
            next_pointer ndptr = bucket_list_[chash];
            if (ndptr != nullptr) {
                for (ndptr = ndptr->next_;
                     ndptr != nullptr &&
                     (ndptr->hash() == hash || detail::constrain_hash(ndptr->hash(), bc) == chash);
                     ndptr = ndptr->next_) {
                    if ((ndptr->hash() == hash) && key_eq_(ndptr->upcast()->get_value(), value))
                        return ndptr;
                }
            }
        }
        if (size() + 1 > bc * max_load_factor() || bc == 0) {
            rehash_unique(std::max<size_type>(
                2 * bc + !detail::is_hash_power2(bc),
                size_type(detail::ht_ceil(float(size() + 1) / max_load_factor()))));
        }
        return nullptr;
    }

    void node_insert_unique_perform(node_pointer nd) noexcept {
        size_type bc = bucket_count();
        size_t chash = detail::constrain_hash(nd->hash(), bc);
        next_pointer pn = bucket_list_[chash];
        if (pn == nullptr) {
            pn = first_node_.ptr();
            nd->next_ = pn->next_;
            pn->next_ = nd->ptr();
            bucket_list_[chash] = pn;
            if (nd->next_ != nullptr)
                bucket_list_[detail::constrain_hash(nd->next_->hash(), bc)] = nd->ptr();
        } else {
            nd->next_ = pn->next_;
            pn->next_ = nd->ptr();
        }
        ++size_;
    }

    // Returns the node after which to insert (for multi), having rehashed.
    next_pointer node_insert_multi_prepare(size_t cp_hash, value_type& cp_val) {
        size_type bc = bucket_count();
        if (size() + 1 > bc * max_load_factor() || bc == 0) {
            rehash_multi(std::max<size_type>(
                2 * bc + !detail::is_hash_power2(bc),
                size_type(detail::ht_ceil(float(size() + 1) / max_load_factor()))));
            bc = bucket_count();
        }
        size_t chash = detail::constrain_hash(cp_hash, bc);
        next_pointer pn = bucket_list_[chash];
        if (pn != nullptr) {
            for (bool found = false;
                 pn->next_ != nullptr && detail::constrain_hash(pn->next_->hash(), bc) == chash;
                 pn = pn->next_) {
                //      found    key_eq()     action
                //      false    false        loop
                //      true     true         loop
                //      false    true         set found to true
                //      true     false        break
                if (found != (pn->next_->hash() == cp_hash &&
                              key_eq_(pn->next_->upcast()->get_value(), cp_val))) {
                    if (!found)
                        found = true;
                    else
                        break;
                }
            }
        }
        return pn;
    }

    void node_insert_multi_perform(node_pointer cp, next_pointer pn) noexcept {
        size_type bc = bucket_count();
        size_t chash = detail::constrain_hash(cp->hash_, bc);
        if (pn == nullptr) {
            pn = first_node_.ptr();
            cp->next_ = pn->next_;
            pn->next_ = cp->ptr();
            bucket_list_[chash] = pn;
            if (cp->next_ != nullptr)
                bucket_list_[detail::constrain_hash(cp->next_->hash(), bc)] = cp->ptr();
        } else {
            cp->next_ = pn->next_;
            pn->next_ = cp->ptr();
            if (cp->next_ != nullptr) {
                size_t nhash = detail::constrain_hash(cp->next_->hash(), bc);
                if (nhash != chash)
                    bucket_list_[nhash] = cp->ptr();
            }
        }
        ++size_;
    }

public:
    pair<iterator, bool> node_insert_unique(node_pointer nd) {
        nd->hash_ = hasher_(nd->get_value());
        next_pointer existing_node = node_insert_unique_prepare(nd->hash(), nd->get_value());

        bool inserted = false;
        if (existing_node == nullptr) {
            node_insert_unique_perform(nd);
            existing_node = nd->ptr();
            inserted = true;
        }
        return pair<iterator, bool>(iterator(existing_node), inserted);
    }

    iterator node_insert_multi(node_pointer cp) {
        cp->hash_ = hasher_(cp->get_value());
        next_pointer pn = node_insert_multi_prepare(cp->hash(), cp->get_value());
        node_insert_multi_perform(cp, pn);
        return iterator(cp->ptr());
    }

    iterator node_insert_multi(const_iterator p, node_pointer cp) {
        if (p != end() && key_eq_(*p, cp->get_value())) {
            next_pointer np = p.node_;
            cp->hash_ = np->hash();
            size_type bc = bucket_count();
            if (size() + 1 > bc * max_load_factor() || bc == 0) {
                rehash_multi(std::max<size_type>(
                    2 * bc + !detail::is_hash_power2(bc),
                    size_type(detail::ht_ceil(float(size() + 1) / max_load_factor()))));
                bc = bucket_count();
            }
            size_t chash = detail::constrain_hash(cp->hash_, bc);
            next_pointer pp = bucket_list_[chash];
            while (pp->next_ != np)
                pp = pp->next_;
            cp->next_ = np;
            pp->next_ = static_cast<next_pointer>(cp->ptr());
            ++size_;
            return iterator(static_cast<next_pointer>(cp->ptr()));
        }
        return node_insert_multi(cp);
    }

    template<class Key, class... Args>
    pair<iterator, bool> emplace_unique_key_args(const Key& k, Args&&... args) {
        size_t hash = hasher_(k);
        size_type bc = bucket_count();
        bool inserted = false;
        next_pointer nd;
        size_t chash;
        if (bc != 0) {
            chash = detail::constrain_hash(hash, bc);
            nd = bucket_list_[chash];
            if (nd != nullptr) {
                for (nd = nd->next_;
                     nd != nullptr &&
                     (nd->hash() == hash || detail::constrain_hash(nd->hash(), bc) == chash);
                     nd = nd->next_) {
                    if ((nd->hash() == hash) && key_eq_(nd->upcast()->get_value(), k))
                        goto done;
                }
            }
        }
        {
            node_holder h = construct_node_hash(hash, std::forward<Args>(args)...);
            if (size() + 1 > bc * max_load_factor() || bc == 0) {
                rehash_unique(std::max<size_type>(
                    2 * bc + !detail::is_hash_power2(bc),
                    size_type(detail::ht_ceil(float(size() + 1) / max_load_factor()))));
                bc = bucket_count();
                chash = detail::constrain_hash(hash, bc);
            }
            next_pointer pn = bucket_list_[chash];
            if (pn == nullptr) {
                pn = first_node_.ptr();
                h->next_ = pn->next_;
                pn->next_ = h.get()->ptr();
                bucket_list_[chash] = pn;
                if (h->next_ != nullptr)
                    bucket_list_[detail::constrain_hash(h->next_->hash(), bc)] = h.get()->ptr();
            } else {
                h->next_ = pn->next_;
                pn->next_ = static_cast<next_pointer>(h.get()->ptr());
            }
            nd = static_cast<next_pointer>(h.release()->ptr());
            ++size_;
            inserted = true;
        }
    done:
        return pair<iterator, bool>(iterator(nd), inserted);
    }

    template<class... Args>
    pair<iterator, bool> emplace_unique_impl(Args&&... args) {
        node_holder h = construct_node(std::forward<Args>(args)...);
        pair<iterator, bool> r = node_insert_unique(h.get());
        if (r.second)
            h.release();
        return r;
    }

    template<class P>
    pair<iterator, bool> emplace_unique_extract_key(P&& x, detail::extract_key_fail_tag) {
        return emplace_unique_impl(std::forward<P>(x));
    }
    template<class P>
    pair<iterator, bool> emplace_unique_extract_key(P&& x, detail::extract_key_self_tag) {
        return emplace_unique_key_args(x, std::forward<P>(x));
    }
    template<class P>
    pair<iterator, bool> emplace_unique_extract_key(P&& x, detail::extract_key_first_tag) {
        return emplace_unique_key_args(x.first, std::forward<P>(x));
    }

    template<class P>
    pair<iterator, bool> emplace_unique(P&& x) {
        return emplace_unique_extract_key(std::forward<P>(x),
                                          detail::ht_can_extract_key<P, key_type>());
    }

    template<class First, class Second,
             enable_if_t<detail::ht_can_extract_map_key<First, key_type,
                                                        container_value_type>::value, int> = 0>
    pair<iterator, bool> emplace_unique(First&& f, Second&& s) {
        return emplace_unique_key_args(f, std::forward<First>(f), std::forward<Second>(s));
    }

    template<class... Args>
    pair<iterator, bool> emplace_unique(Args&&... args) {
        return emplace_unique_impl(std::forward<Args>(args)...);
    }

    template<class... Args>
    iterator emplace_multi(Args&&... args) {
        node_holder h = construct_node(std::forward<Args>(args)...);
        iterator r = node_insert_multi(h.get());
        h.release();
        return r;
    }

    template<class... Args>
    iterator emplace_hint_multi(const_iterator p, Args&&... args) {
        node_holder h = construct_node(std::forward<Args>(args)...);
        iterator r = node_insert_multi(p, h.get());
        h.release();
        return r;
    }

    pair<iterator, bool> insert_unique(container_value_type&& x) {
        return emplace_unique_key_args(kv_types::get_key(x), std::move(x));
    }

    pair<iterator, bool> insert_unique(const container_value_type& x) {
        return emplace_unique_key_args(kv_types::get_key(x), x);
    }

    template<class P, enable_if_t<!is_same_v<remove_const_t<remove_reference_t<P>>,
                                             container_value_type>, int> = 0>
    pair<iterator, bool> insert_unique(P&& x) {
        return emplace_unique(std::forward<P>(x));
    }

    template<class P>
    iterator insert_multi(P&& x) {
        return emplace_multi(std::forward<P>(x));
    }

    template<class P>
    iterator insert_multi(const_iterator p, P&& x) {
        return emplace_hint_multi(p, std::forward<P>(x));
    }

    // ------------------------------------------------------------------
    // Node handle operations
    // ------------------------------------------------------------------

    template<class NodeHandle, class InsertReturnType>
    InsertReturnType node_handle_insert_unique(NodeHandle&& nh) {
        if (nh.empty())
            return InsertReturnType{end(), false, NodeHandle()};
        pair<iterator, bool> result = node_insert_unique(nh.ptr_);
        if (result.second)
            nh.release_ptr();
        return InsertReturnType{result.first, result.second, std::move(nh)};
    }

    template<class NodeHandle>
    iterator node_handle_insert_unique(const_iterator, NodeHandle&& nh) {
        if (nh.empty())
            return end();
        pair<iterator, bool> result = node_insert_unique(nh.ptr_);
        if (result.second)
            nh.release_ptr();
        return result.first;
    }

    template<class NodeHandle>
    NodeHandle node_handle_extract(const key_type& key) {
        iterator i = find(key);
        if (i == end())
            return NodeHandle();
        return node_handle_extract<NodeHandle>(i);
    }

    template<class NodeHandle>
    NodeHandle node_handle_extract(const_iterator p) {
        allocator_type alloc(node_alloc());
        return NodeHandle(remove(p).release(), alloc);
    }

    template<class Table>
    void node_handle_merge_unique(Table& source) {
        static_assert(is_same_v<node, typename Table::node>);
        for (typename Table::iterator it = source.begin(); it != source.end();) {
            node_pointer src_ptr = it.node_->upcast();
            size_t hash = hasher_(src_ptr->get_value());
            next_pointer existing_node = node_insert_unique_prepare(hash, src_ptr->get_value());
            auto prev_iter = it++;
            if (existing_node == nullptr) {
                (void)source.remove(prev_iter).release();
                src_ptr->hash_ = hash;
                node_insert_unique_perform(src_ptr);
            }
        }
    }

    template<class NodeHandle>
    iterator node_handle_insert_multi(NodeHandle&& nh) {
        if (nh.empty())
            return end();
        iterator result = node_insert_multi(nh.ptr_);
        nh.release_ptr();
        return result;
    }

    template<class NodeHandle>
    iterator node_handle_insert_multi(const_iterator hint, NodeHandle&& nh) {
        if (nh.empty())
            return end();
        iterator result = node_insert_multi(hint, nh.ptr_);
        nh.release_ptr();
        return result;
    }

    template<class Table>
    void node_handle_merge_multi(Table& source) {
        static_assert(is_same_v<typename Table::node, node>);
        for (typename Table::iterator it = source.begin(); it != source.end();) {
            node_pointer src_ptr = it.node_->upcast();
            size_t src_hash = hasher_(src_ptr->get_value());
            next_pointer pn = node_insert_multi_prepare(src_hash, src_ptr->get_value());
            (void)source.remove(it++).release();
            src_ptr->hash_ = src_hash;
            node_insert_multi_perform(src_ptr, pn);
        }
    }

    // ------------------------------------------------------------------
    // clear / rehash
    // ------------------------------------------------------------------

    void clear() noexcept {
        if (size() > 0) {
            deallocate_node(first_node_.next_);
            first_node_.next_ = nullptr;
            size_type bc = bucket_count();
            for (size_type i = 0; i < bc; ++i)
                bucket_list_[i] = nullptr;
            size_ = 0;
        }
    }

    void rehash_unique(size_type n) { rehash<true>(n); }
    void rehash_multi(size_type n) { rehash<false>(n); }
    void reserve_unique(size_type n) {
        rehash_unique(static_cast<size_type>(detail::ht_ceil(float(n) / max_load_factor())));
    }
    void reserve_multi(size_type n) {
        rehash_multi(static_cast<size_type>(detail::ht_ceil(float(n) / max_load_factor())));
    }

private:
    template<bool UniqueKeys>
    void rehash(size_type n) {
        if (n == 1)
            n = 2;
        else if (n & (n - 1))
            n = detail::next_prime(n);
        size_type bc = bucket_count();
        if (n > bc)
            do_rehash<UniqueKeys>(n);
        else if (n < bc) {
            n = std::max<size_type>(
                n,
                detail::is_hash_power2(bc)
                    ? detail::next_hash_pow2(size_t(detail::ht_ceil(float(size()) / max_load_factor())))
                    : detail::next_prime(size_t(detail::ht_ceil(float(size()) / max_load_factor()))));
            if (n < bc)
                do_rehash<UniqueKeys>(n);
        }
    }

    template<bool UniqueKeys>
    void do_rehash(size_type nbc) {
        // Allocate the new bucket list first (libc++ resets the unique_ptr,
        // which deallocates the old list with the old size, then installs the
        // new one).
        pointer_allocator pa(node_alloc_);
        bucket_list_pointer new_list = nbc > 0 ? pointer_alloc_traits::allocate(pa, nbc) : nullptr;
        if (bucket_list_ != nullptr)
            pointer_alloc_traits::deallocate(pa, bucket_list_, bucket_count_);
        bucket_list_ = new_list;
        bucket_count_ = nbc;
        if (nbc > 0) {
            for (size_type i = 0; i < nbc; ++i)
                bucket_list_[i] = nullptr;
            next_pointer pp = first_node_.ptr();
            next_pointer cp = pp->next_;
            if (cp != nullptr) {
                size_type chash = detail::constrain_hash(cp->hash(), nbc);
                bucket_list_[chash] = pp;
                size_type phash = chash;
                for (pp = cp, void(), cp = cp->next_; cp != nullptr; cp = pp->next_) {
                    chash = detail::constrain_hash(cp->hash(), nbc);
                    if (chash == phash)
                        pp = cp;
                    else {
                        if (bucket_list_[chash] == nullptr) {
                            bucket_list_[chash] = pp;
                            pp = cp;
                            phash = chash;
                        } else {
                            next_pointer np = cp;
                            if constexpr (!UniqueKeys) {
                                for (; np->next_ != nullptr &&
                                       key_eq_(cp->upcast()->get_value(),
                                               np->next_->upcast()->get_value());
                                     np = np->next_)
                                    ;
                            }
                            pp->next_ = np->next_;
                            np->next_ = bucket_list_[chash]->next_;
                            bucket_list_[chash]->next_ = cp;
                        }
                    }
                }
            }
        }
    }

public:
    // ------------------------------------------------------------------
    // Iterators
    // ------------------------------------------------------------------

    iterator begin() noexcept { return iterator(first_node_.next_); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept { return const_iterator(first_node_.next_); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }

    // ------------------------------------------------------------------
    // Lookup
    // ------------------------------------------------------------------

    template<class Key>
    size_type bucket(const Key& k) const {
        return detail::constrain_hash(hash_function()(k), bucket_count());
    }

    template<class Key>
    iterator find(const Key& k) {
        size_t hash = hasher_(k);
        size_type bc = bucket_count();
        if (bc != 0) {
            size_t chash = detail::constrain_hash(hash, bc);
            next_pointer nd = bucket_list_[chash];
            if (nd != nullptr) {
                for (nd = nd->next_;
                     nd != nullptr &&
                     (nd->hash() == hash || detail::constrain_hash(nd->hash(), bc) == chash);
                     nd = nd->next_) {
                    if ((nd->hash() == hash) && key_eq_(nd->upcast()->get_value(), k))
                        return iterator(nd);
                }
            }
        }
        return end();
    }

    template<class Key>
    const_iterator find(const Key& k) const {
        size_t hash = hasher_(k);
        size_type bc = bucket_count();
        if (bc != 0) {
            size_t chash = detail::constrain_hash(hash, bc);
            next_pointer nd = bucket_list_[chash];
            if (nd != nullptr) {
                for (nd = nd->next_;
                     nd != nullptr &&
                     (hash == nd->hash() || detail::constrain_hash(nd->hash(), bc) == chash);
                     nd = nd->next_) {
                    if ((nd->hash() == hash) && key_eq_(nd->upcast()->get_value(), k))
                        return const_iterator(nd);
                }
            }
        }
        return end();
    }

    template<class Key>
    size_type count_unique(const Key& k) const {
        return static_cast<size_type>(find(k) != end());
    }

    template<class Key>
    size_type count_multi(const Key& k) const {
        size_type r = 0;
        const_iterator i = find(k);
        if (i != end()) {
            const_iterator e = end();
            do {
                ++i;
                ++r;
            } while (i != e && key_eq_(*i, k));
        }
        return r;
    }

    template<class Key>
    pair<iterator, iterator> equal_range_unique(const Key& k) {
        iterator i = find(k);
        iterator j = i;
        if (i != end())
            ++j;
        return pair<iterator, iterator>(i, j);
    }

    template<class Key>
    pair<const_iterator, const_iterator> equal_range_unique(const Key& k) const {
        const_iterator i = find(k);
        const_iterator j = i;
        if (i != end())
            ++j;
        return pair<const_iterator, const_iterator>(i, j);
    }

    template<class Key>
    pair<iterator, iterator> equal_range_multi(const Key& k) {
        iterator i = find(k);
        iterator j = i;
        if (i != end()) {
            iterator e = end();
            do {
                ++j;
            } while (j != e && key_eq_(*j, k));
        }
        return pair<iterator, iterator>(i, j);
    }

    template<class Key>
    pair<const_iterator, const_iterator> equal_range_multi(const Key& k) const {
        const_iterator i = find(k);
        const_iterator j = i;
        if (i != end()) {
            const_iterator e = end();
            do {
                ++j;
            } while (j != e && key_eq_(*j, k));
        }
        return pair<const_iterator, const_iterator>(i, j);
    }

    // ------------------------------------------------------------------
    // Construct nodes
    // ------------------------------------------------------------------

    template<class... Args>
    node_holder construct_node(Args&&... args) {
        node_allocator& na = node_alloc();
        node_holder h(node_traits::allocate(na, 1), node_destructor(na));
        // Begin the lifetime of the node itself; the value is constructed
        // separately via the allocator (allocator-aware).
        std::construct_at(std::addressof(*h), /*next =*/nullptr, /*hash =*/size_t(0));
        node_traits::construct(na, kv_types::get_ptr(h->get_value()), std::forward<Args>(args)...);
        h.get_deleter().value_constructed = true;
        h->hash_ = hasher_(h->get_value());
        return h;
    }

    template<class First, class... Rest>
    node_holder construct_node_hash(size_t hash, First&& f, Rest&&... rest) {
        node_allocator& na = node_alloc();
        node_holder h(node_traits::allocate(na, 1), node_destructor(na));
        std::construct_at(std::addressof(*h), /*next =*/nullptr, /*hash =*/hash);
        node_traits::construct(na, kv_types::get_ptr(h->get_value()),
                               std::forward<First>(f), std::forward<Rest>(rest)...);
        h.get_deleter().value_constructed = true;
        return h;
    }

    // ------------------------------------------------------------------
    // Erase
    // ------------------------------------------------------------------

    iterator erase(const_iterator p) {
        next_pointer np = p.node_;
        iterator r(np);
        ++r;
        remove(p);
        return r;
    }

    iterator erase(const_iterator first, const_iterator last) {
        for (const_iterator p = first; first != last; p = first) {
            ++first;
            erase(p);
        }
        next_pointer np = last.node_;
        return iterator(np);
    }

    template<class Key>
    size_type erase_unique(const Key& k) {
        iterator i = find(k);
        if (i == end())
            return 0;
        erase(i);
        return 1;
    }

    template<class Key>
    size_type erase_multi(const Key& k) {
        size_type r = 0;
        iterator i = find(k);
        if (i != end()) {
            iterator e = end();
            do {
                erase(i++);
                ++r;
            } while (i != e && key_eq_(*i, k));
        }
        return r;
    }

    node_holder remove(const_iterator p) noexcept {
        // current node
        next_pointer cn = p.node_;
        size_type bc = bucket_count();
        size_t chash = detail::constrain_hash(cn->hash(), bc);
        // find previous node
        next_pointer pn = bucket_list_[chash];
        for (; pn->next_ != cn; pn = pn->next_)
            ;
        // Fix up bucket_list_
        if (pn == first_node_.ptr() || detail::constrain_hash(pn->hash(), bc) != chash) {
            if (cn->next_ == nullptr || detail::constrain_hash(cn->next_->hash(), bc) != chash)
                bucket_list_[chash] = nullptr;
        }
        if (cn->next_ != nullptr) {
            size_t nhash = detail::constrain_hash(cn->next_->hash(), bc);
            if (nhash != chash)
                bucket_list_[nhash] = pn;
        }
        // remove cn
        pn->next_ = cn->next_;
        cn->next_ = nullptr;
        --size_;
        return node_holder(cn->upcast(), node_destructor(node_alloc(), true));
    }

    // ------------------------------------------------------------------
    // Swap
    // ------------------------------------------------------------------

    void swap(hash_table& u) noexcept(
        is_nothrow_swappable_v<hasher> && is_nothrow_swappable_v<key_equal>) {
        using std::swap;
        swap(bucket_list_, u.bucket_list_);
        swap(bucket_count_, u.bucket_count_);
        if constexpr (node_traits::propagate_on_container_swap::value)
            swap(node_alloc_, u.node_alloc_);
        swap(first_node_.next_, u.first_node_.next_);
        swap(size_, u.size_);
        swap(hasher_, u.hasher_);
        swap(max_load_factor_, u.max_load_factor_);
        swap(key_eq_, u.key_eq_);
        if (size() > 0)
            bucket_list_[detail::constrain_hash(first_node_.next_->hash(), bucket_count())] =
                first_node_.ptr();
        if (u.size() > 0)
            u.bucket_list_[detail::constrain_hash(u.first_node_.next_->hash(), u.bucket_count())] =
                u.first_node_.ptr();
    }

    // ------------------------------------------------------------------
    // Bucket interface / load factor
    // ------------------------------------------------------------------

    size_type max_bucket_count() const noexcept { return max_size(); }

    size_type bucket_size(size_type n) const {
        next_pointer np = bucket_list_[n];
        size_type bc = bucket_count();
        size_type r = 0;
        if (np != nullptr) {
            for (np = np->next_;
                 np != nullptr && detail::constrain_hash(np->hash(), bc) == n;
                 np = np->next_, (void)++r)
                ;
        }
        return r;
    }

    float load_factor() const noexcept {
        size_type bc = bucket_count();
        return bc != 0 ? (float)size() / bc : 0.f;
    }

    void max_load_factor(float mlf) noexcept {
        max_load_factor_ = std::max(mlf, load_factor());
    }

    local_iterator begin(size_type n) {
        return local_iterator(bucket_list_[n], n, bucket_count());
    }
    local_iterator end(size_type n) {
        return local_iterator(nullptr, n, bucket_count());
    }
    const_local_iterator cbegin(size_type n) const {
        return const_local_iterator(bucket_list_[n], n, bucket_count());
    }
    const_local_iterator cend(size_type n) const {
        return const_local_iterator(nullptr, n, bucket_count());
    }
};

template<class Tp, class Hash, class Equal, class Alloc>
inline void swap(hash_table<Tp, Hash, Equal, Alloc>& x, hash_table<Tp, Hash, Equal, Alloc>& y)
    noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

} // namespace std
