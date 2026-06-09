// bits/hash_table.h — internal hash table infrastructure
// Ported from libc++ include/__hash_table
// Shared by <unordered_map> and <unordered_set>.
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <type_traits>
#include <limits>
#include <cstddef>

namespace std {

// ============================================================================
// Internal helpers
// ============================================================================

namespace detail {

inline bool is_hash_power2(size_t bc) noexcept {
    return bc > 2 && !(bc & (bc - 1));
}

inline size_t constrain_hash(size_t h, size_t bc) noexcept {
    return !(bc & (bc - 1)) ? (h & (bc - 1)) : (h < bc ? h : h % bc);
}

// Next power-of-two >= n
inline size_t next_hash_pow2(size_t n) noexcept {
    if (n <= 1) return 1;
    unsigned long long v = (unsigned long long)(n - 1);
    int lz = __builtin_clzll(v | 1ULL);
    return size_t(1) << (64 - lz);
}

// Simple next-prime
inline size_t next_prime(size_t n) noexcept {
    if (n <= 2) return 2;
    if (n <= 3) return 3;
    if (!(n & 1)) ++n;
    while (true) {
        bool ok = true;
        for (size_t d = 3; d * d <= n; d += 2) {
            if (n % d == 0) { ok = false; break; }
        }
        if (ok) return n;
        n += 2;
    }
}

inline float ceil_f(float f) noexcept {
    long long i = static_cast<long long>(f);
    return (f > static_cast<float>(i)) ? static_cast<float>(i + 1) : static_cast<float>(i);
}

} // namespace detail

// ============================================================================
// Node types
// ============================================================================

// Forward declaration
template<class Tp, class Hash, class Equal, class Alloc, bool Unique>
class hash_table;

// Base node.
// NodePtr = pointer to ht_node<Tp,VoidPtr>.
// next_ is a pointer to ht_node_base (the "base pointer" / "next pointer" type).
// This matches libc++'s __hash_node_base design where __next_ is __next_pointer
// (a rebind of NodePtr to the base type).
template<class NodePtr>
struct ht_node_base {
    // The "base pointer" type = pointer to ht_node_base, rebound from NodePtr
    using base_ptr_type = typename pointer_traits<NodePtr>::template rebind<ht_node_base>;

    base_ptr_type next_;   // pointer to next base node (== next in singly-linked list)

    ht_node_base() noexcept : next_(nullptr) {}

    // Convert this base to the base_ptr_type pointing to *this
    base_ptr_type ptr() noexcept {
        return pointer_traits<base_ptr_type>::pointer_to(*this);
    }

    // Upcast: interpret *this as the full node and return NodePtr
    // Valid when *this is actually a ht_node (not the sentinel head)
    NodePtr upcast() noexcept {
        return static_cast<NodePtr>(
            pointer_traits<base_ptr_type>::pointer_to(*this));
    }
};

// Full node: inherits base (which carries next_), plus hash and value
template<class Tp, class VoidPtr>
struct ht_node
    : ht_node_base<typename pointer_traits<VoidPtr>::template rebind<ht_node<Tp,VoidPtr>>>
{
    using value_type    = Tp;
    using node_ptr_type = typename pointer_traits<VoidPtr>::template rebind<ht_node>;

    size_t hash_val;

private:
    union { value_type val_; };  // deferred initialisation
public:
    value_type&       get_value()       noexcept { return val_; }
    const value_type& get_value() const noexcept { return val_; }
    ~ht_node() {}
};

// ============================================================================
// Iterators
//
// Both mutable and const iterators store a base_ptr_type (pointer to the
// base node).  Dereferencing casts via upcast() to get the value.
// Incrementing advances via next_.
// ============================================================================

template<class NodePtr>
class ht_iterator {
    using Node      = typename pointer_traits<NodePtr>::element_type;
    using BaseType  = ht_node_base<NodePtr>;
    using BasePtr   = typename pointer_traits<NodePtr>::template rebind<BaseType>;
public:
    using iterator_category = forward_iterator_tag;
    using value_type        = typename Node::value_type;
    using difference_type   = ptrdiff_t;
    using reference         = value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<value_type>;

    BasePtr node_;

    ht_iterator() noexcept : node_(nullptr) {}
    explicit ht_iterator(BasePtr n) noexcept : node_(n) {}

    reference operator*()  const noexcept {
        return static_cast<NodePtr>(node_)->get_value();
    }
    pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(operator*());
    }
    ht_iterator& operator++() noexcept { node_ = node_->next_; return *this; }
    ht_iterator  operator++(int) noexcept { ht_iterator t(*this); ++(*this); return t; }

    friend bool operator==(const ht_iterator& x, const ht_iterator& y) noexcept { return x.node_ == y.node_; }
    friend bool operator!=(const ht_iterator& x, const ht_iterator& y) noexcept { return !(x==y); }

    template<class,class,class,class,bool> friend class hash_table;
};

template<class NodePtr>
class ht_const_iterator {
    using Node      = typename pointer_traits<NodePtr>::element_type;
    using BaseType  = ht_node_base<NodePtr>;
    using BasePtr   = typename pointer_traits<NodePtr>::template rebind<BaseType>;
public:
    using iterator_category = forward_iterator_tag;
    using value_type        = typename Node::value_type;
    using difference_type   = ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<const value_type>;

    BasePtr node_;

    ht_const_iterator() noexcept : node_(nullptr) {}
    explicit ht_const_iterator(BasePtr n) noexcept : node_(n) {}
    ht_const_iterator(const ht_iterator<NodePtr>& it) noexcept : node_(it.node_) {}

    reference operator*()  const noexcept {
        return static_cast<NodePtr>(node_)->get_value();
    }
    pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(operator*());
    }
    ht_const_iterator& operator++() noexcept { node_ = node_->next_; return *this; }
    ht_const_iterator  operator++(int) noexcept { ht_const_iterator t(*this); ++(*this); return t; }

    friend bool operator==(const ht_const_iterator& x, const ht_const_iterator& y) noexcept { return x.node_ == y.node_; }
    friend bool operator!=(const ht_const_iterator& x, const ht_const_iterator& y) noexcept { return !(x==y); }

    template<class,class,class,class,bool> friend class hash_table;
};

template<class NodePtr>
class ht_local_iterator {
    using Node      = typename pointer_traits<NodePtr>::element_type;
    using BaseType  = ht_node_base<NodePtr>;
    using BasePtr   = typename pointer_traits<NodePtr>::template rebind<BaseType>;
public:
    using iterator_category = forward_iterator_tag;
    using value_type        = typename Node::value_type;
    using difference_type   = ptrdiff_t;
    using reference         = value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<value_type>;

    BasePtr  node_;
    size_t   bucket_idx_;
    size_t   bucket_cnt_;

    ht_local_iterator() noexcept : node_(nullptr), bucket_idx_(0), bucket_cnt_(0) {}
    // Construct from predecessor bp: first node is bp->next_
    explicit ht_local_iterator(BasePtr bp, size_t b, size_t bc) noexcept
        : node_(bp ? bp->next_ : nullptr), bucket_idx_(b), bucket_cnt_(bc) {}

    reference operator*()  const noexcept { return static_cast<NodePtr>(node_)->get_value(); }
    pointer   operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(operator*());
    }
    ht_local_iterator& operator++() noexcept {
        node_ = node_->next_;
        if (node_ &&
            detail::constrain_hash(static_cast<NodePtr>(node_)->hash_val, bucket_cnt_) != bucket_idx_)
            node_ = nullptr;
        return *this;
    }
    ht_local_iterator operator++(int) noexcept { ht_local_iterator t(*this); ++(*this); return t; }
    friend bool operator==(const ht_local_iterator& x, const ht_local_iterator& y) noexcept { return x.node_ == y.node_; }
    friend bool operator!=(const ht_local_iterator& x, const ht_local_iterator& y) noexcept { return !(x==y); }

    template<class,class,class,class,bool> friend class hash_table;
    template<class> friend class ht_const_local_iterator;
};

template<class NodePtr>
class ht_const_local_iterator {
    using Node      = typename pointer_traits<NodePtr>::element_type;
    using BaseType  = ht_node_base<NodePtr>;
    using BasePtr   = typename pointer_traits<NodePtr>::template rebind<BaseType>;
public:
    using iterator_category = forward_iterator_tag;
    using value_type        = typename Node::value_type;
    using difference_type   = ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = typename pointer_traits<NodePtr>::template rebind<const value_type>;

    BasePtr  node_;
    size_t   bucket_idx_;
    size_t   bucket_cnt_;

    ht_const_local_iterator() noexcept : node_(nullptr), bucket_idx_(0), bucket_cnt_(0) {}
    explicit ht_const_local_iterator(BasePtr bp, size_t b, size_t bc) noexcept
        : node_(bp ? bp->next_ : nullptr), bucket_idx_(b), bucket_cnt_(bc) {}
    ht_const_local_iterator(const ht_local_iterator<NodePtr>& it) noexcept
        : node_(it.node_), bucket_idx_(it.bucket_idx_), bucket_cnt_(it.bucket_cnt_) {}

    reference operator*()  const noexcept { return static_cast<NodePtr>(node_)->get_value(); }
    pointer   operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(operator*());
    }
    ht_const_local_iterator& operator++() noexcept {
        node_ = node_->next_;
        if (node_ &&
            detail::constrain_hash(static_cast<NodePtr>(node_)->hash_val, bucket_cnt_) != bucket_idx_)
            node_ = nullptr;
        return *this;
    }
    ht_const_local_iterator operator++(int) noexcept { ht_const_local_iterator t(*this); ++(*this); return t; }
    friend bool operator==(const ht_const_local_iterator& x, const ht_const_local_iterator& y) noexcept { return x.node_ == y.node_; }
    friend bool operator!=(const ht_const_local_iterator& x, const ht_const_local_iterator& y) noexcept { return !(x==y); }

    template<class,class,class,class,bool> friend class hash_table;
};

// ============================================================================
// Node destructor
// ============================================================================

template<class NodeAlloc>
struct ht_node_destructor {
    using traits   = allocator_traits<NodeAlloc>;
    using pointer  = typename traits::pointer;
    NodeAlloc& alloc;
    bool       value_constructed;
    explicit ht_node_destructor(NodeAlloc& a, bool vc=false) noexcept : alloc(a), value_constructed(vc) {}
    void operator()(pointer p) noexcept {
        if (value_constructed && p)
            traits::destroy(alloc, addressof(p->get_value()));
        if (p)
            traits::deallocate(alloc, p, 1);
    }
};

// ============================================================================
// hash_table
// ============================================================================

template<class Tp, class Hash, class Equal, class Alloc, bool Unique>
class hash_table {
public:
    using value_type      = Tp;
    using hasher          = Hash;
    using key_equal       = Equal;
    using allocator_type  = Alloc;

    using alloc_traits    = allocator_traits<Alloc>;
    using size_type       = typename alloc_traits::size_type;
    using difference_type = ptrdiff_t;
    using pointer         = typename alloc_traits::pointer;
    using const_pointer   = typename alloc_traits::const_pointer;

    using void_ptr    = typename alloc_traits::void_pointer;
    using node_type   = ht_node<Tp, void_ptr>;
    using node_alloc  = typename alloc_traits::template rebind_alloc<node_type>;
    using node_traits = allocator_traits<node_alloc>;
    using node_ptr    = typename node_traits::pointer;        // = node_type*

    // base_node = ht_node_base<node_ptr>
    // base_ptr  = pointer to base_node (= ht_node_base<node_ptr>*)
    using base_node   = ht_node_base<node_ptr>;
    using base_ptr    = typename base_node::base_ptr_type;    // = base_node*

    // Bucket array: each entry is a base_ptr (predecessor pointer to first node)
    using bucket_alloc  = typename alloc_traits::template rebind_alloc<base_ptr>;
    using bucket_traits = allocator_traits<bucket_alloc>;
    using bucket_arr    = typename bucket_traits::pointer;    // = base_ptr*

    using node_dtor   = ht_node_destructor<node_alloc>;
    using node_holder = unique_ptr<node_type, node_dtor>;

    using iterator             = ht_iterator<node_ptr>;
    using const_iterator       = ht_const_iterator<node_ptr>;
    using local_iterator       = ht_local_iterator<node_ptr>;
    using const_local_iterator = ht_const_local_iterator<node_ptr>;

public:
    // --- data members ---
    bucket_arr buckets_;
    size_type  bucket_count_;
    base_node  head_;         // sentinel; head_.next_ = base_ptr to first real node
    node_alloc node_alloc_;
    size_type  size_;
    float      max_load_factor_;
    hasher     hasher_;
    key_equal  key_eq_;

    // ---- utilities ----

    base_ptr head_as_base() noexcept {
        return pointer_traits<base_ptr>::pointer_to(head_);
    }

    // Convert node_ptr to base_ptr (valid: node_type inherits from base_node)
    static base_ptr node_to_base(node_ptr np) noexcept {
        return pointer_traits<base_ptr>::pointer_to(
            *static_cast<base_node*>(np));
    }

    // ---- bucket array alloc/free ----

    void alloc_buckets(size_type n) {
        bucket_alloc ba(node_alloc_);
        buckets_      = bucket_traits::allocate(ba, n);
        bucket_count_ = n;
        for (size_type i = 0; i < n; ++i)
            buckets_[i] = nullptr;
    }

    void free_buckets() noexcept {
        if (buckets_) {
            bucket_alloc ba(node_alloc_);
            bucket_traits::deallocate(ba, buckets_, bucket_count_);
            buckets_      = nullptr;
            bucket_count_ = 0;
        }
    }

    // ---- node construction ----

    template<class... Args>
    node_holder construct_node(Args&&... args) {
        node_holder h(node_traits::allocate(node_alloc_, 1),
                      node_dtor(node_alloc_, false));
        h->next_    = nullptr;
        h->hash_val = 0;
        node_traits::construct(node_alloc_, addressof(h->get_value()),
                               forward<Args>(args)...);
        h.get_deleter().value_constructed = true;
        h->hash_val = hasher_(h->get_value());
        return h;
    }

    template<class... Args>
    node_holder construct_node_with_hash(size_t hv, Args&&... args) {
        node_holder h(node_traits::allocate(node_alloc_, 1),
                      node_dtor(node_alloc_, false));
        h->next_    = nullptr;
        h->hash_val = hv;
        node_traits::construct(node_alloc_, addressof(h->get_value()),
                               forward<Args>(args)...);
        h.get_deleter().value_constructed = true;
        return h;
    }

    void deallocate_node(node_ptr np) noexcept {
        node_traits::destroy(node_alloc_, addressof(np->get_value()));
        node_traits::deallocate(node_alloc_, np, 1);
    }

    // Deallocate entire chain starting at bp (base_ptr)
    void deallocate_list(base_ptr bp) noexcept {
        while (bp) {
            base_ptr nxt = static_cast<node_ptr>(bp)->next_;
            deallocate_node(static_cast<node_ptr>(bp));
            bp = nxt;
        }
    }

    // ---- grow if needed ----
    void maybe_grow() {
        size_type bc = bucket_count_;
        size_type s1 = 2*bc + (detail::is_hash_power2(bc) ? 0 : 1);
        size_type s2 = (size_type)detail::ceil_f((float)(size_+1) / max_load_factor_);
        size_type n  = s1 > s2 ? s1 : s2;
        if (n < 2) n = 2;
        do_rehash(n);
    }

    // ---- raw insert unique ----
    // Returns (base_ptr_to_node, was_inserted)
    pair<base_ptr, bool> raw_insert_unique(node_ptr np) noexcept {
        size_t hv = np->hash_val;
        size_t ch = detail::constrain_hash(hv, bucket_count_);
        base_ptr bp = buckets_[ch];
        if (bp) {
            for (base_ptr nd = bp->next_; nd; nd = static_cast<node_ptr>(nd)->next_) {
                node_ptr ep = static_cast<node_ptr>(nd);
                if (ep->hash_val != hv &&
                    detail::constrain_hash(ep->hash_val, bucket_count_) != ch)
                    break;
                if (ep->hash_val == hv && key_eq_(ep->get_value(), np->get_value()))
                    return {nd, false};
            }
            // Insert after bp (bp is predecessor of the bucket's chain)
            np->next_  = bp->next_;
            bp->next_  = node_to_base(np);
            if (np->next_) {
                size_t nch = detail::constrain_hash(
                    static_cast<node_ptr>(np->next_)->hash_val, bucket_count_);
                if (nch != ch) buckets_[nch] = node_to_base(np);
            }
        } else {
            // Bucket empty: insert at head of the global list
            base_ptr hbp = head_as_base();
            np->next_  = hbp->next_;
            hbp->next_ = node_to_base(np);
            buckets_[ch] = hbp;
            if (np->next_) {
                size_t nch = detail::constrain_hash(
                    static_cast<node_ptr>(np->next_)->hash_val, bucket_count_);
                buckets_[nch] = node_to_base(np);
            }
        }
        ++size_;
        return {node_to_base(np), true};
    }

    // ---- find multi insertion point ----
    base_ptr find_multi_pos(size_t hv, const value_type& val) noexcept {
        size_t ch = detail::constrain_hash(hv, bucket_count_);
        base_ptr bp = buckets_[ch];
        if (!bp) return nullptr;
        bool found = false;
        for (base_ptr nd = bp; nd->next_; nd = nd->next_) {
            node_ptr np2 = static_cast<node_ptr>(nd->next_);
            if (detail::constrain_hash(np2->hash_val, bucket_count_) != ch)
                break;
            bool eq = (np2->hash_val == hv) && key_eq_(np2->get_value(), val);
            if (found != eq) {
                if (!found) found = true;
                else        return nd;
            }
        }
        return bp;
    }

    void raw_insert_multi(node_ptr np) noexcept {
        size_t hv = np->hash_val;
        size_t ch = detail::constrain_hash(hv, bucket_count_);
        base_ptr bp = find_multi_pos(hv, np->get_value());
        if (!bp) {
            base_ptr hbp = head_as_base();
            np->next_  = hbp->next_;
            hbp->next_ = node_to_base(np);
            buckets_[ch] = hbp;
            if (np->next_) {
                size_t nch = detail::constrain_hash(
                    static_cast<node_ptr>(np->next_)->hash_val, bucket_count_);
                buckets_[nch] = node_to_base(np);
            }
        } else {
            np->next_  = bp->next_;
            bp->next_  = node_to_base(np);
            if (np->next_) {
                size_t nch = detail::constrain_hash(
                    static_cast<node_ptr>(np->next_)->hash_val, bucket_count_);
                if (nch != ch) buckets_[nch] = node_to_base(np);
            }
        }
        ++size_;
    }

    // ---- rehash ----
    void do_rehash(size_type n) {
        free_buckets();
        if (n == 0) return;
        alloc_buckets(n);

        // Rebuild bucket_ entries from the existing linked list
        // head_.next_ is the base_ptr to the first real node
        base_ptr pp = head_as_base();
        if (!pp->next_) return;

        {
            node_ptr first = static_cast<node_ptr>(pp->next_);
            size_t ch_cur = detail::constrain_hash(first->hash_val, n);
            buckets_[ch_cur] = pp;
            size_t ch_prev   = ch_cur;
            pp = pp->next_;

            for (base_ptr cp = pp->next_; cp; cp = pp->next_) {
                ch_cur = detail::constrain_hash(static_cast<node_ptr>(cp)->hash_val, n);
                if (ch_cur == ch_prev) {
                    pp = cp;
                } else if (!buckets_[ch_cur]) {
                    buckets_[ch_cur] = pp;
                    pp      = cp;
                    ch_prev = ch_cur;
                } else {
                    // Move this node (and equal-key cluster for multi) to existing bucket
                    base_ptr tail = cp;
                    if constexpr (!Unique) {
                        while (tail->next_ &&
                               key_eq_(static_cast<node_ptr>(cp)->get_value(),
                                       static_cast<node_ptr>(tail->next_)->get_value()))
                            tail = tail->next_;
                    }
                    pp->next_               = tail->next_;
                    tail->next_             = buckets_[ch_cur]->next_;
                    buckets_[ch_cur]->next_ = cp;
                }
            }
        }
    }

    // ---- copy ----
    void copy_from(const hash_table& src) {
        if (src.size_ == 0) return;
        do_rehash(src.bucket_count_);

        // Walk source list; build a new linked list in the same order
        base_ptr tail = head_as_base();
        // src.head_.next_ is base_ptr to first real node in src
        for (base_ptr sp = src.head_.next_; sp; sp = static_cast<node_ptr>(sp)->next_) {
            node_ptr sn = static_cast<node_ptr>(sp);
            node_holder h = construct_node_with_hash(sn->hash_val, sn->get_value());
            node_ptr np = h.release();
            np->next_   = nullptr;
            tail->next_ = node_to_base(np);   // base_ptr = base_ptr
            tail        = node_to_base(np);    // advance tail
            ++size_;
        }
        // Rebuild bucket predecessor pointers
        rebuild_bucket_heads();
    }

    void rebuild_bucket_heads() noexcept {
        for (size_type i = 0; i < bucket_count_; ++i)
            buckets_[i] = nullptr;
        base_ptr pp = head_as_base();
        for (base_ptr cp = pp->next_; cp; cp = static_cast<node_ptr>(cp)->next_) {
            size_t ch = detail::constrain_hash(
                static_cast<node_ptr>(cp)->hash_val, bucket_count_);
            if (!buckets_[ch]) buckets_[ch] = pp;
            pp = cp;
        }
    }

public:
    // =========================================================================
    // Constructors / destructor
    // =========================================================================

    hash_table() noexcept
        : buckets_(nullptr), bucket_count_(0), head_(), node_alloc_(),
          size_(0), max_load_factor_(1.0f), hasher_(), key_eq_()
    {}

    hash_table(size_type n, const hasher& hf, const key_equal& eq,
               const allocator_type& a = allocator_type())
        : buckets_(nullptr), bucket_count_(0), head_(), node_alloc_(a),
          size_(0), max_load_factor_(1.0f), hasher_(hf), key_eq_(eq)
    {
        if (n > 0) {
            size_type bc = detail::next_prime(n);
            alloc_buckets(bc);
        }
    }

    explicit hash_table(const allocator_type& a)
        : buckets_(nullptr), bucket_count_(0), head_(), node_alloc_(a),
          size_(0), max_load_factor_(1.0f), hasher_(), key_eq_()
    {}

    hash_table(const hash_table& src)
        : buckets_(nullptr), bucket_count_(0), head_(), node_alloc_(src.node_alloc_),
          size_(0), max_load_factor_(src.max_load_factor_),
          hasher_(src.hasher_), key_eq_(src.key_eq_)
    { copy_from(src); }

    hash_table(const hash_table& src, const allocator_type& a)
        : buckets_(nullptr), bucket_count_(0), head_(), node_alloc_(a),
          size_(0), max_load_factor_(src.max_load_factor_),
          hasher_(src.hasher_), key_eq_(src.key_eq_)
    { copy_from(src); }

    hash_table(hash_table&& src) noexcept
        : buckets_(src.buckets_), bucket_count_(src.bucket_count_),
          head_(), node_alloc_(move(src.node_alloc_)),
          size_(src.size_), max_load_factor_(src.max_load_factor_),
          hasher_(move(src.hasher_)), key_eq_(move(src.key_eq_))
    {
        head_.next_        = src.head_.next_;
        src.head_.next_    = nullptr;
        src.buckets_       = nullptr;
        src.bucket_count_  = 0;
        src.size_          = 0;
        // Fix the bucket that pointed to src's head sentinel → now point to our head
        if (size_ > 0 && bucket_count_ > 0 && head_.next_) {
            size_t ch = detail::constrain_hash(
                static_cast<node_ptr>(head_.next_)->hash_val, bucket_count_);
            buckets_[ch] = head_as_base();
        }
    }

    hash_table(hash_table&& src, const allocator_type& a)
        : buckets_(nullptr), bucket_count_(0), head_(), node_alloc_(a),
          size_(0), max_load_factor_(src.max_load_factor_),
          hasher_(move(src.hasher_)), key_eq_(move(src.key_eq_))
    {
        if (a == allocator_type(src.node_alloc_)) {
            buckets_           = src.buckets_;
            bucket_count_      = src.bucket_count_;
            head_.next_        = src.head_.next_;
            size_              = src.size_;
            src.buckets_       = nullptr;
            src.bucket_count_  = 0;
            src.head_.next_    = nullptr;
            src.size_          = 0;
            if (size_ > 0 && bucket_count_ > 0 && head_.next_) {
                size_t ch = detail::constrain_hash(
                    static_cast<node_ptr>(head_.next_)->hash_val, bucket_count_);
                buckets_[ch] = head_as_base();
            }
        } else {
            while (src.size_ > 0) {
                auto it = src.begin();
                auto h  = src.extract_node(it);
                h->hash_val = hasher_(h->get_value());
                if (bucket_count_ == 0 || size_+1 > bucket_count_*max_load_factor_)
                    maybe_grow();
                if constexpr (Unique) raw_insert_unique(h.release());
                else                  raw_insert_multi(h.release());
            }
        }
    }

    ~hash_table() {
        deallocate_list(head_.next_);
        head_.next_ = nullptr;
        free_buckets();
    }

    // =========================================================================
    // Assignment
    // =========================================================================

    hash_table& operator=(const hash_table& src) {
        if (this == &src) return *this;
        clear();
        hasher_          = src.hasher_;
        key_eq_          = src.key_eq_;
        max_load_factor_ = src.max_load_factor_;
        if constexpr (node_traits::propagate_on_container_copy_assignment::value)
            node_alloc_ = src.node_alloc_;
        copy_from(src);
        return *this;
    }

    hash_table& operator=(hash_table&& src) noexcept(
        is_nothrow_move_assignable_v<hasher> &&
        is_nothrow_move_assignable_v<key_equal> &&
        (node_traits::propagate_on_container_move_assignment::value ||
         node_traits::is_always_equal::value))
    {
        if (this == &src) return *this;
        bool steal = node_traits::propagate_on_container_move_assignment::value ||
                     node_alloc_ == src.node_alloc_;
        if (steal) {
            clear();
            free_buckets();
            if constexpr (node_traits::propagate_on_container_move_assignment::value)
                node_alloc_ = move(src.node_alloc_);
            buckets_          = src.buckets_;
            bucket_count_     = src.bucket_count_;
            head_.next_       = src.head_.next_;
            size_             = src.size_;
            hasher_           = move(src.hasher_);
            key_eq_           = move(src.key_eq_);
            max_load_factor_  = src.max_load_factor_;
            src.buckets_      = nullptr;
            src.bucket_count_ = 0;
            src.head_.next_   = nullptr;
            src.size_         = 0;
            if (size_ > 0 && bucket_count_ > 0 && head_.next_) {
                size_t ch = detail::constrain_hash(
                    static_cast<node_ptr>(head_.next_)->hash_val, bucket_count_);
                buckets_[ch] = head_as_base();
            }
        } else {
            clear();
            hasher_          = move(src.hasher_);
            key_eq_          = move(src.key_eq_);
            max_load_factor_ = src.max_load_factor_;
            while (src.size_ > 0) {
                auto it  = src.begin();
                auto h   = src.extract_node(it);
                value_type v = move(h->get_value());
                node_traits::destroy(src.node_alloc_, addressof(h->get_value()));
                h.get_deleter().value_constructed = false;
                emplace_impl(move(v));
            }
        }
        return *this;
    }

    // =========================================================================
    // Iterators
    // =========================================================================

    iterator       begin()        noexcept { return iterator(head_.next_); }
    iterator       end()          noexcept { return iterator(nullptr); }
    const_iterator begin()  const noexcept {
        // head_.next_ is already a base_ptr; cast away const
        return const_iterator(const_cast<base_ptr>(head_.next_));
    }
    const_iterator end()    const noexcept { return const_iterator(nullptr); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend()   const noexcept { return end(); }

    // =========================================================================
    // Capacity
    // =========================================================================

    bool      empty()    const noexcept { return size_ == 0; }
    size_type size()     const noexcept { return size_; }
    size_type max_size() const noexcept {
        return min<size_type>(node_traits::max_size(node_alloc_),
                              (size_type)numeric_limits<difference_type>::max());
    }

    // =========================================================================
    // Observers
    // =========================================================================

    hasher&          hash_function()       noexcept { return hasher_; }
    const hasher&    hash_function() const noexcept { return hasher_; }
    key_equal&       key_eq()              noexcept { return key_eq_; }
    const key_equal& key_eq()       const  noexcept { return key_eq_; }
    allocator_type   get_allocator() const noexcept { return allocator_type(node_alloc_); }

    // =========================================================================
    // Emplace / insert
    // =========================================================================

    template<class... Args>
    pair<iterator,bool> emplace_unique(Args&&... args) {
        node_holder h = construct_node(forward<Args>(args)...);
        if (bucket_count_ == 0 || size_+1 > (size_type)(bucket_count_*max_load_factor_))
            maybe_grow();
        auto [bp, ins] = raw_insert_unique(h.get());
        if (ins) h.release();
        return {iterator(bp), ins};
    }

    template<class... Args>
    iterator emplace_multi(Args&&... args) {
        node_holder h = construct_node(forward<Args>(args)...);
        if (bucket_count_ == 0 || size_+1 > (size_type)(bucket_count_*max_load_factor_))
            maybe_grow();
        node_ptr np = h.release();
        raw_insert_multi(np);
        return iterator(node_to_base(np));
    }

    // Dispatch unique/multi
    template<class... Args>
    auto emplace_impl(Args&&... args) {
        if constexpr (Unique) return emplace_unique(forward<Args>(args)...);
        else                  return emplace_multi(forward<Args>(args)...);
    }

    // Insert pre-built node (node already has hash_val set)
    pair<iterator,bool> insert_node_unique(node_ptr np) {
        if (bucket_count_ == 0 || size_+1 > (size_type)(bucket_count_*max_load_factor_))
            maybe_grow();
        auto [bp, ins] = raw_insert_unique(np);
        return {iterator(bp), ins};
    }

    iterator insert_node_multi(node_ptr np) {
        if (bucket_count_ == 0 || size_+1 > (size_type)(bucket_count_*max_load_factor_))
            maybe_grow();
        raw_insert_multi(np);
        return iterator(node_to_base(np));
    }

    // =========================================================================
    // Lookup
    // =========================================================================
private:
    template<class K>
    base_ptr find_base(const K& k) const {
        if (bucket_count_ == 0 || size_ == 0) return nullptr;
        size_t hv = hasher_(k);
        size_t ch = detail::constrain_hash(hv, bucket_count_);
        base_ptr bp = buckets_[ch];
        if (!bp) return nullptr;
        for (base_ptr nd = bp->next_; nd; nd = static_cast<node_ptr>(nd)->next_) {
            node_ptr np = static_cast<node_ptr>(nd);
            if (np->hash_val != hv &&
                detail::constrain_hash(np->hash_val, bucket_count_) != ch)
                break;
            if (np->hash_val == hv && key_eq_(np->get_value(), k))
                return nd;
        }
        return nullptr;
    }

public:
    iterator       find(const value_type& k)       { auto p = find_base(k); return p ? iterator(p) : end(); }
    const_iterator find(const value_type& k) const { auto p = find_base(k); return p ? const_iterator(p) : end(); }

    size_type count_unique(const value_type& k) const { return find_base(k) ? 1 : 0; }
    size_type count_multi(const value_type& k) const {
        size_type r = 0;
        for (auto i = find(k); i != end() && key_eq_(*i, k); ++i) ++r;
        return r;
    }

    pair<iterator,iterator> equal_range_unique(const value_type& k) {
        iterator i = find(k), j = i;
        if (i != end()) ++j;
        return {i, j};
    }
    pair<const_iterator,const_iterator> equal_range_unique(const value_type& k) const {
        const_iterator i = find(k), j = i;
        if (i != end()) ++j;
        return {i, j};
    }
    pair<iterator,iterator> equal_range_multi(const value_type& k) {
        iterator i = find(k), j = i;
        if (i != end()) { auto e = end(); do { ++j; } while (j != e && key_eq_(*j, k)); }
        return {i, j};
    }
    pair<const_iterator,const_iterator> equal_range_multi(const value_type& k) const {
        const_iterator i = find(k), j = i;
        if (i != end()) { auto e = end(); do { ++j; } while (j != e && key_eq_(*j, k)); }
        return {i, j};
    }

    // =========================================================================
    // Erase
    // =========================================================================

    iterator erase(const_iterator pos) {
        base_ptr cn = pos.node_;
        iterator r(cn); ++r;
        extract_node(pos);
        return r;
    }

    iterator erase(const_iterator first, const_iterator last) {
        while (first != last) first = const_iterator(erase(first).node_);
        return iterator(last.node_);
    }

    size_type erase_unique(const value_type& k) {
        auto i = find(k);
        if (i == end()) return 0;
        erase(i);
        return 1;
    }

    size_type erase_multi(const value_type& k) {
        size_type r = 0;
        auto i = find(k);
        while (i != end() && key_eq_(*i, k)) { i = erase(i); ++r; }
        return r;
    }

    // Extract node; returns holder owning it (value not yet deallocated)
    node_holder extract_node(const_iterator pos) noexcept {
        base_ptr cn = pos.node_;
        node_ptr np = static_cast<node_ptr>(cn);
        size_t hv   = np->hash_val;
        size_t ch   = detail::constrain_hash(hv, bucket_count_);

        // Find predecessor in the global list
        // buckets_[ch] is the predecessor of the first node in bucket ch
        base_ptr pn = buckets_[ch];
        while (pn && pn->next_ != cn)
            pn = pn->next_;

        // Fix bucket heads
        bool pn_in_same = (pn != head_as_base() &&
            detail::constrain_hash(static_cast<node_ptr>(pn)->hash_val, bucket_count_) == ch);
        if (!pn_in_same) {
            if (!cn->next_ ||
                detail::constrain_hash(static_cast<node_ptr>(cn->next_)->hash_val, bucket_count_) != ch)
                buckets_[ch] = nullptr;
        }
        if (cn->next_) {
            size_t nch = detail::constrain_hash(
                static_cast<node_ptr>(cn->next_)->hash_val, bucket_count_);
            if (nch != ch) buckets_[nch] = pn;
        }
        pn->next_  = cn->next_;
        np->next_  = nullptr;
        --size_;
        return node_holder(np, node_dtor(node_alloc_, true));
    }

    // =========================================================================
    // Clear
    // =========================================================================

    void clear() noexcept {
        if (size_ > 0) {
            deallocate_list(head_.next_);
            head_.next_ = nullptr;
            for (size_type i = 0; i < bucket_count_; ++i)
                buckets_[i] = nullptr;
            size_ = 0;
        }
    }

    // =========================================================================
    // Bucket interface
    // =========================================================================

    size_type bucket_count()     const noexcept { return bucket_count_; }
    size_type max_bucket_count() const noexcept { return max_size(); }

    size_type bucket_size(size_type n) const noexcept {
        if (n >= bucket_count_) return 0;
        base_ptr bp = buckets_[n];
        if (!bp) return 0;
        size_type r = 0;
        for (base_ptr nd = bp->next_;
             nd && detail::constrain_hash(static_cast<node_ptr>(nd)->hash_val, bucket_count_) == n;
             nd = static_cast<node_ptr>(nd)->next_)
            ++r;
        return r;
    }

    template<class K>
    size_type bucket(const K& k) const {
        if (!bucket_count_) return 0;
        return detail::constrain_hash(hasher_(k), bucket_count_);
    }

    local_iterator begin(size_type n) noexcept {
        if (n >= bucket_count_ || !buckets_[n]) return local_iterator();
        return local_iterator(buckets_[n], n, bucket_count_);
    }
    local_iterator end(size_type) noexcept { return local_iterator(); }

    const_local_iterator begin(size_type n) const noexcept {
        if (n >= bucket_count_ || !buckets_[n]) return const_local_iterator();
        return const_local_iterator(buckets_[n], n, bucket_count_);
    }
    const_local_iterator end(size_type) const noexcept { return const_local_iterator(); }
    const_local_iterator cbegin(size_type n) const noexcept { return begin(n); }
    const_local_iterator cend(size_type n)   const noexcept { return end(n); }

    // =========================================================================
    // Load factor / rehash / reserve
    // =========================================================================

    float load_factor()     const noexcept {
        return bucket_count_ ? (float)size_ / (float)bucket_count_ : 0.f;
    }
    float max_load_factor() const noexcept { return max_load_factor_; }
    void  max_load_factor(float mlf) noexcept {
        if (mlf > 0)
            max_load_factor_ = mlf;
    }

    void rehash(size_type n) {
        size_type min_req = (size_type)detail::ceil_f((float)size_ / max_load_factor_);
        if (n < min_req) n = min_req;
        if (n <= 1) n = 2;
        if (n != bucket_count_)
            do_rehash(n);
    }

    void reserve(size_type n) {
        rehash((size_type)detail::ceil_f((float)n / max_load_factor_));
    }

    // =========================================================================
    // Swap
    // =========================================================================

    void swap(hash_table& other) noexcept(
        is_nothrow_swappable_v<hasher> && is_nothrow_swappable_v<key_equal>)
    {
        using std::swap;
        swap(buckets_,         other.buckets_);
        swap(bucket_count_,    other.bucket_count_);
        swap(head_.next_,      other.head_.next_);
        swap(node_alloc_,      other.node_alloc_);
        swap(size_,            other.size_);
        swap(max_load_factor_, other.max_load_factor_);
        swap(hasher_,          other.hasher_);
        swap(key_eq_,          other.key_eq_);
        // Fix head-sentinel bucket pointers after swap
        if (size_ > 0 && bucket_count_ > 0 && head_.next_) {
            size_t ch = detail::constrain_hash(
                static_cast<node_ptr>(head_.next_)->hash_val, bucket_count_);
            buckets_[ch] = head_as_base();
        }
        if (other.size_ > 0 && other.bucket_count_ > 0 && other.head_.next_) {
            size_t ch = detail::constrain_hash(
                static_cast<node_ptr>(other.head_.next_)->hash_val, other.bucket_count_);
            other.buckets_[ch] = other.head_as_base();
        }
    }

    // =========================================================================
    // Node handles — extract and merge
    // =========================================================================

    node_holder extract(const_iterator pos) noexcept { return extract_node(pos); }

    node_holder extract_by_key(const value_type& k) noexcept {
        auto i = find(k);
        if (i == end())
            return node_holder(nullptr, node_dtor(node_alloc_, false));
        return extract_node(i);
    }

    template<bool SrcUnique>
    void merge_unique(hash_table<Tp,Hash,Equal,Alloc,SrcUnique>& src) {
        for (auto it = src.begin(); it != src.end(); ) {
            auto cur = it++;
            if (find(*cur) == end()) {
                auto h = src.extract_node(cur);
                node_ptr moved = h.release();
                moved->hash_val = hasher_(moved->get_value());
                if (bucket_count_ == 0 || size_+1 > (size_type)(bucket_count_*max_load_factor_))
                    maybe_grow();
                raw_insert_unique(moved);
            }
        }
    }

    template<bool SrcUnique>
    void merge_multi(hash_table<Tp,Hash,Equal,Alloc,SrcUnique>& src) {
        for (auto it = src.begin(); it != src.end(); ) {
            auto cur = it++;
            auto h = src.extract_node(cur);
            node_ptr moved = h.release();
            moved->hash_val = hasher_(moved->get_value());
            if (bucket_count_ == 0 || size_+1 > (size_type)(bucket_count_*max_load_factor_))
                maybe_grow();
            raw_insert_multi(moved);
        }
    }

    // =========================================================================
    // Equality
    // =========================================================================

    friend bool operator==(const hash_table& a, const hash_table& b) {
        if (a.size_ != b.size_) return false;
        if constexpr (Unique) {
            for (auto& v : a) {
                if (!b.find_base(v)) return false;
            }
            return true;
        } else {
            for (auto it = a.begin(); it != a.end(); ) {
                const value_type& v = *it;
                size_type n1 = 0, n2 = 0;
                auto j = it;
                while (j != a.end() && a.key_eq_(v, *j)) { ++j; ++n1; }
                auto [b1, b2] = b.equal_range_multi(v);
                for (auto k = b1; k != b2; ++k) ++n2;
                if (n1 != n2) return false;
                it = j;
            }
            return true;
        }
    }

    // =========================================================================
    // Assign from iterator range
    // =========================================================================

    template<class InputIt>
    void assign_unique(InputIt first, InputIt last) {
        clear();
        for (; first != last; ++first)
            emplace_unique(*first);
    }

    template<class InputIt>
    void assign_multi(InputIt first, InputIt last) {
        clear();
        for (; first != last; ++first)
            emplace_multi(*first);
    }
};

} // namespace std
