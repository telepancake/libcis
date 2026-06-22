// bits/tree.h — Red-black tree infrastructure shared by map and set.
// Mechanically ported from libc++ include/__tree
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
//
// namespace std — legitimate bits/ header (shared by map and set; would cause
// a cycle if inlined into both user headers directly).
#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <iterator>
#include <initializer_list>
#include <limits>
#include <type_traits>
#include <cstdint>    // uintptr_t — color packed into parent pointer's low bit

namespace std {

namespace detail {
// libc++ __is_allocator: constrains map/set deduction guides so a
// non-allocator argument makes deduction fail (SFINAE) instead of
// hard-erroring inside allocator_traits.
template<class Alloc>
inline constexpr bool tree_is_allocator_v = requires(Alloc& a) {
    typename Alloc::value_type;
    a.allocate(size_t{});
};
} // namespace detail

// ============================================================
// Node structures
//
// Memory layout (using raw pointers, void* allocator):
//
//   rb_end_node<node_base*>   — has: node_base* left
//     rb_node_base<void*>     — inherits rb_end_node; has: node_base* right,
//                               end_node* parent, bool is_black
//       rb_node<T, void*>     — inherits rb_node_base; has: T value (union)
//
// The sentinel end_node_ is of type rb_end_node.
// Its left pointer is the root of the tree.
// A node's parent pointer points to either the sentinel or another node.
// Because rb_node_base IS-A rb_end_node, a node* can serve as an end_node*
// (the sentinel uses only the .left field anyway).
// ============================================================

// Forward declarations
template<class NBP> struct rb_end_node;
template<class VP>  class rb_node_base;
template<class T, class VP> class rb_node;

// rb_end_node: holds only the left (root) pointer
template<class NodeBasePtr>
struct rb_end_node {
    NodeBasePtr left = nullptr;
};

// rb_node_base: full node structure
template<class VoidPtr>
class rb_node_base
    : public rb_end_node<typename pointer_traits<VoidPtr>::template rebind<rb_node_base<VoidPtr>>>
{
public:
    using node_base_ptr  = typename pointer_traits<VoidPtr>::template rebind<rb_node_base>;
    using end_node_t     = rb_end_node<node_base_ptr>;
    using end_node_ptr   = typename pointer_traits<VoidPtr>::template rebind<end_node_t>;

    node_base_ptr right    = nullptr;
    // parent + color packed into one word. The pointer's low bit carries the
    // red/black color (rb nodes are at least pointer-aligned, so the low 3 bits
    // are always zero in a real parent pointer); bit 0 set == BLACK. This drops
    // the separate `bool is_black` field, which cost a full 8 bytes per node
    // after alignment padding before the value union — a ~20% node shrink for
    // map<int,int>. EVERY parent read masks the bit off; every parent write
    // preserves the current color. The raw word is private so no path can read
    // `parent` without going through the masking accessors.
private:
    end_node_ptr parent = nullptr;

    static constexpr unsigned color_bit = 1u;
    // Reinterpret the parent word as an integer to manipulate the low bit.
    uintptr_t parent_bits() const noexcept {
        uintptr_t v; __builtin_memcpy(&v, &parent, sizeof(v)); return v;
    }
    void store_parent_bits(uintptr_t v) noexcept {
        __builtin_memcpy(&parent, &v, sizeof(parent));
    }

public:
    rb_node_base() = default;
    rb_node_base(const rb_node_base&) = delete;
    rb_node_base& operator=(const rb_node_base&) = delete;

    // ---- color (low bit of parent) ----
    bool black() const noexcept { return parent_bits() & color_bit; }
    void set_black(bool b) noexcept {
        store_parent_bits((parent_bits() & ~uintptr_t(color_bit)) | (b ? color_bit : 0));
    }

    // ---- parent (high bits of parent), color-masked on every access ----
    // Read the parent as an end_node_ptr (may be the sentinel). Masks color.
    end_node_ptr get_parent_end() const noexcept {
        end_node_ptr p; uintptr_t v = parent_bits() & ~uintptr_t(color_bit);
        __builtin_memcpy(&p, &v, sizeof(p)); return p;
    }
    // Read the parent as a node_base_ptr. Masks color.
    node_base_ptr parent_unsafe() const noexcept {
        node_base_ptr r; uintptr_t v = parent_bits() & ~uintptr_t(color_bit);
        __builtin_memcpy(&r, &v, sizeof(r)); return r;
    }
    // Store a node_base_ptr as parent, preserving the current color bit.
    void set_parent(node_base_ptr p) noexcept {
        uintptr_t v; __builtin_memcpy(&v, &p, sizeof(v));
        store_parent_bits((v & ~uintptr_t(color_bit)) | (parent_bits() & color_bit));
    }
    // Store an end_node_ptr (e.g. root's parent = sentinel), preserving color.
    void set_parent_end(end_node_ptr ep) noexcept {
        uintptr_t v; __builtin_memcpy(&v, &ep, sizeof(v));
        store_parent_bits((v & ~uintptr_t(color_bit)) | (parent_bits() & color_bit));
    }
    // Copy the parent pointer (NOT the color) from another node into this one.
    void copy_parent_from(const rb_node_base& o) noexcept {
        store_parent_bits((o.parent_bits() & ~uintptr_t(color_bit))
                          | (parent_bits() & color_bit));
    }
    // Fresh-node init: parent = null, color = red. Writes the WHOLE word, so it
    // is safe on an allocated-but-uninitialized node (no read of stale color).
    void init_parent_red() noexcept { store_parent_bits(0); }
};

// rb_node: adds the stored value
template<class T, class VoidPtr>
class rb_node : public rb_node_base<VoidPtr> {
public:
    union { T value; };
    rb_node()  {}
    ~rb_node() {}
    rb_node(const rb_node&)            = delete;
    rb_node& operator=(const rb_node&) = delete;
    T&       get_value()       noexcept { return value; }
    const T& get_value() const noexcept { return value; }
};

// Move a stored value out of a node (libc++ _NodeTypes::__move): for map
// values pair<const K, M> the const key must be cast away so the key can be
// moved rather than copied.
template<class V>
struct rb_value_mover {
    static V&& do_move(V& v) noexcept { return static_cast<V&&>(v); }
};
template<class F, class S>
struct rb_value_mover<pair<const F, S>> {
    static pair<F&&, S&&> do_move(pair<const F, S>& v) noexcept {
        return pair<F&&, S&&>(static_cast<F&&>(const_cast<F&>(v.first)),
                              static_cast<S&&>(v.second));
    }
};

// ============================================================
// Pointer cast helper (same-size raw pointers)
// ============================================================
template<class To, class From>
To rbtree_cast(From p) noexcept {
    static_assert(sizeof(To) == sizeof(From), "pointer sizes must match");
    To r; __builtin_memcpy(&r, &p, sizeof(r)); return r;
}

// ============================================================
// Tree structural algorithms — operate on node_base_ptr
// ============================================================

template<class NP> // NP = node_base_ptr
inline bool tree_is_left_child(NP x) noexcept {
    return x == x->get_parent_end()->left;
}

template<class NP>
NP tree_min(NP x) noexcept {
    while (x->left) x = static_cast<NP>(x->left);
    return x;
}

template<class NP>
NP tree_max(NP x) noexcept {
    while (x->right) x = static_cast<NP>(x->right);
    return x;
}

// tree_next_iter: EndNP is end_node_ptr (can hold sentinel or real node)
template<class EndNP, class NP>
EndNP tree_next_iter(NP x) noexcept {
    if (x->right) {
        NP m = static_cast<NP>(x->right);
        while (m->left) m = static_cast<NP>(m->left);
        return rbtree_cast<EndNP>(m);
    }
    while (!tree_is_left_child(x))
        x = x->parent_unsafe();
    return x->get_parent_end();  // parent is end_node_ptr
}

// tree_prev_iter: EndNP can be sentinel
template<class NP, class EndNP>
NP tree_prev_iter(EndNP x) noexcept {
    if (x->left) {
        NP m = rbtree_cast<NP>(x->left);
        while (m->right) m = static_cast<NP>(m->right);
        return m;
    }
    NP xx = rbtree_cast<NP>(x);
    while (tree_is_left_child(xx))
        xx = xx->parent_unsafe();
    return xx->parent_unsafe();
}

template<class NP>
void tree_left_rotate(NP x) noexcept {
    NP y = static_cast<NP>(x->right);
    x->right = y->left;
    if (x->right) static_cast<NP>(x->right)->set_parent(x);
    y->copy_parent_from(*x);
    if (tree_is_left_child(x))
        x->get_parent_end()->left = y;
    else
        x->parent_unsafe()->right = y;
    y->left = x;
    x->set_parent(y);
}

template<class NP>
void tree_right_rotate(NP x) noexcept {
    NP y = static_cast<NP>(x->left);
    x->left = y->right;
    if (x->left) static_cast<NP>(x->left)->set_parent(x);
    y->copy_parent_from(*x);
    if (tree_is_left_child(x))
        x->get_parent_end()->left = y;
    else
        x->parent_unsafe()->right = y;
    y->right = x;
    x->set_parent(y);
}

template<class NP>
void tree_balance_after_insert(NP root, NP x) noexcept {
    x->set_black(x == root);
    while (x != root && !x->parent_unsafe()->black()) {
        if (tree_is_left_child(x->parent_unsafe())) {
            NP uncle = x->parent_unsafe()->parent_unsafe()->right
                     ? static_cast<NP>(x->parent_unsafe()->parent_unsafe()->right) : nullptr;
            if (uncle && !uncle->black()) {
                x = x->parent_unsafe(); x->set_black(true);
                x = x->parent_unsafe(); x->set_black(x == root);
                uncle->set_black(true);
            } else {
                if (!tree_is_left_child(x)) {
                    x = x->parent_unsafe();
                    tree_left_rotate(x);
                }
                x = x->parent_unsafe(); x->set_black(true);
                x = x->parent_unsafe(); x->set_black(false);
                tree_right_rotate(x);
                break;
            }
        } else {
            NP uncle = x->parent_unsafe()->parent_unsafe()->left
                     ? static_cast<NP>(x->parent_unsafe()->parent_unsafe()->left) : nullptr;
            if (uncle && !uncle->black()) {
                x = x->parent_unsafe(); x->set_black(true);
                x = x->parent_unsafe(); x->set_black(x == root);
                uncle->set_black(true);
            } else {
                if (tree_is_left_child(x)) {
                    x = x->parent_unsafe();
                    tree_right_rotate(x);
                }
                x = x->parent_unsafe(); x->set_black(true);
                x = x->parent_unsafe(); x->set_black(false);
                tree_left_rotate(x);
                break;
            }
        }
    }
}

// tree_remove: unlink z from tree; update root_ref if root changes
template<class NP>
void tree_remove(NP& root_ref, NP z) noexcept {
    NP root = root_ref;
    NP y = (z->left == nullptr || z->right == nullptr) ? z : [&]{
        NP n = static_cast<NP>(z->right);
        while (n->left) n = static_cast<NP>(n->left);
        return n;
    }();
    NP x = y->left  ? static_cast<NP>(y->left)
         : y->right ? static_cast<NP>(y->right) : nullptr;
    NP w = nullptr;

    if (x) x->copy_parent_from(*y);
    if (tree_is_left_child(y)) {
        y->get_parent_end()->left = x;
        if (y != root) w = static_cast<NP>(y->parent_unsafe()->right);
        else           root = x;
    } else {
        y->parent_unsafe()->right = x;
        w = static_cast<NP>(y->get_parent_end()->left);
    }
    bool removed_black = y->black();
    if (y != z) {
        y->copy_parent_from(*z);
        if (tree_is_left_child(z)) y->get_parent_end()->left = y;
        else                       y->parent_unsafe()->right = y;
        y->left = z->left;
        static_cast<NP>(y->left)->set_parent(y);
        y->right = z->right;
        if (y->right) static_cast<NP>(y->right)->set_parent(y);
        y->set_black(z->black());
        if (root == z) root = y;
    }
    root_ref = root;
    if (!removed_black || root == nullptr) return;
    if (x) { x->set_black(true); return; }

    while (true) {
        if (!tree_is_left_child(w)) {
            if (!w->black()) {
                w->set_black(true);
                w->parent_unsafe()->set_black(false);
                tree_left_rotate(w->parent_unsafe());
                if (root == w->left) root = w;
                w = static_cast<NP>(static_cast<NP>(w->left)->right);
            }
            if ((!w->left  || static_cast<NP>(w->left)->black()) &&
                (!w->right || static_cast<NP>(w->right)->black())) {
                w->set_black(false);
                x = w->parent_unsafe();
                if (x == root || !x->black()) { x->set_black(true); break; }
                w = tree_is_left_child(x)
                  ? static_cast<NP>(x->parent_unsafe()->right)
                  : static_cast<NP>(x->get_parent_end()->left);
            } else {
                if (!w->right || static_cast<NP>(w->right)->black()) {
                    static_cast<NP>(w->left)->set_black(true);
                    w->set_black(false);
                    tree_right_rotate(w);
                    w = w->parent_unsafe();
                }
                w->set_black(w->parent_unsafe()->black());
                w->parent_unsafe()->set_black(true);
                static_cast<NP>(w->right)->set_black(true);
                tree_left_rotate(w->parent_unsafe());
                if (root == w->left) root = w;
                break;
            }
        } else {
            if (!w->black()) {
                w->set_black(true);
                w->parent_unsafe()->set_black(false);
                tree_right_rotate(w->parent_unsafe());
                if (root == w->right) root = w;
                w = static_cast<NP>(static_cast<NP>(w->right)->left);
            }
            if ((!w->left  || static_cast<NP>(w->left)->black()) &&
                (!w->right || static_cast<NP>(w->right)->black())) {
                w->set_black(false);
                x = w->parent_unsafe();
                if (!x->black() || x == root) { x->set_black(true); break; }
                w = tree_is_left_child(x)
                  ? static_cast<NP>(x->parent_unsafe()->right)
                  : static_cast<NP>(x->get_parent_end()->left);
            } else {
                if (!w->left || static_cast<NP>(w->left)->black()) {
                    static_cast<NP>(w->right)->set_black(true);
                    w->set_black(false);
                    tree_left_rotate(w);
                    w = w->parent_unsafe();
                }
                w->set_black(w->parent_unsafe()->black());
                w->parent_unsafe()->set_black(true);
                static_cast<NP>(w->left)->set_black(true);
                tree_right_rotate(w->parent_unsafe());
                if (root == w->right) root = w;
                break;
            }
        }
    }
    root_ref = root;
}

// ============================================================
// rb_tree_iterator / rb_tree_const_iterator
// ============================================================

template<class T, class VP, class Diff> class rb_tree_const_iterator;

// Pointer typedefs computed from the void pointer alone — must NOT require
// rb_node<T, VP> to be complete (incomplete-type support: map<A, A> inside A).
template<class T, class VP>
struct rb_tree_ptr_types {
    using node_pointer  = typename pointer_traits<VP>::template rebind<rb_node<T, VP>>;
    using node_base_ptr = typename pointer_traits<VP>::template rebind<rb_node_base<VP>>;
    using end_node_ptr  = typename pointer_traits<VP>::template rebind<rb_end_node<node_base_ptr>>;
};

template<class T, class VP, class Diff>
class rb_tree_iterator {
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = Diff;
    using reference         = T&;
    using pointer           = typename pointer_traits<VP>::template rebind<T>;

    using node_pointer  = typename rb_tree_ptr_types<T, VP>::node_pointer;
    using node_base_ptr = typename rb_tree_ptr_types<T, VP>::node_base_ptr;
    using end_node_ptr  = typename rb_tree_ptr_types<T, VP>::end_node_ptr;

private:
    end_node_ptr ptr_;   // points to either a real node or the sentinel end_node

    friend class rb_tree_const_iterator<T, VP, Diff>;
    template<class, class, class> friend class rb_tree;

    explicit rb_tree_iterator(node_pointer np) noexcept
        : ptr_(rbtree_cast<end_node_ptr>(np)) {}
    explicit rb_tree_iterator(end_node_ptr ep) noexcept : ptr_(ep) {}
    explicit rb_tree_iterator(node_base_ptr nbp) noexcept
        : ptr_(rbtree_cast<end_node_ptr>(nbp)) {}

public:
    rb_tree_iterator() noexcept : ptr_(nullptr) {}

    node_pointer get_node() const noexcept { return rbtree_cast<node_pointer>(ptr_); }
    node_base_ptr get_base() const noexcept { return rbtree_cast<node_base_ptr>(ptr_); }

    reference operator*()  const noexcept { return get_node()->get_value(); }
    pointer   operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(get_node()->get_value());
    }

    rb_tree_iterator& operator++() noexcept {
        ptr_ = tree_next_iter<end_node_ptr>(get_base());
        return *this;
    }
    rb_tree_iterator operator++(int) noexcept { auto t = *this; ++*this; return t; }

    rb_tree_iterator& operator--() noexcept {
        node_base_ptr np = tree_prev_iter<node_base_ptr>(ptr_);
        ptr_ = rbtree_cast<end_node_ptr>(np);
        return *this;
    }
    rb_tree_iterator operator--(int) noexcept { auto t = *this; --*this; return t; }

    friend bool operator==(const rb_tree_iterator& a, const rb_tree_iterator& b) noexcept {
        return a.ptr_ == b.ptr_;
    }
    friend bool operator!=(const rb_tree_iterator& a, const rb_tree_iterator& b) noexcept {
        return !(a == b);
    }
};

template<class T, class VP, class Diff>
class rb_tree_const_iterator {
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = Diff;
    using reference         = const T&;
    using pointer           = typename pointer_traits<VP>::template rebind<const T>;

    using node_pointer       = typename rb_tree_ptr_types<T, VP>::node_pointer;
    using non_const_iterator = rb_tree_iterator<T, VP, Diff>;
    using node_base_ptr      = typename rb_tree_ptr_types<T, VP>::node_base_ptr;
    using end_node_ptr       = typename rb_tree_ptr_types<T, VP>::end_node_ptr;

private:
    end_node_ptr ptr_;

    template<class, class, class> friend class rb_tree;

    explicit rb_tree_const_iterator(node_pointer np) noexcept
        : ptr_(rbtree_cast<end_node_ptr>(np)) {}
    explicit rb_tree_const_iterator(end_node_ptr ep) noexcept : ptr_(ep) {}
    explicit rb_tree_const_iterator(node_base_ptr nbp) noexcept
        : ptr_(rbtree_cast<end_node_ptr>(nbp)) {}

public:
    rb_tree_const_iterator() noexcept : ptr_(nullptr) {}
    rb_tree_const_iterator(non_const_iterator it) noexcept : ptr_(it.ptr_) {}

    node_pointer get_node() const noexcept { return rbtree_cast<node_pointer>(ptr_); }
    node_base_ptr get_base() const noexcept { return rbtree_cast<node_base_ptr>(ptr_); }

    reference operator*()  const noexcept { return get_node()->get_value(); }
    pointer   operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(get_node()->get_value());
    }

    rb_tree_const_iterator& operator++() noexcept {
        ptr_ = tree_next_iter<end_node_ptr>(get_base());
        return *this;
    }
    rb_tree_const_iterator operator++(int) noexcept { auto t = *this; ++*this; return t; }

    rb_tree_const_iterator& operator--() noexcept {
        node_base_ptr np = tree_prev_iter<node_base_ptr>(ptr_);
        ptr_ = rbtree_cast<end_node_ptr>(np);
        return *this;
    }
    rb_tree_const_iterator operator--(int) noexcept { auto t = *this; --*this; return t; }

    friend bool operator==(const rb_tree_const_iterator& a, const rb_tree_const_iterator& b) noexcept {
        return a.ptr_ == b.ptr_;
    }
    friend bool operator!=(const rb_tree_const_iterator& a, const rb_tree_const_iterator& b) noexcept {
        return !(a == b);
    }
};

// ============================================================
// rb_tree
// ============================================================

template<class T, class Compare, class Alloc>
class rb_tree {
public:
    using value_type     = T;
    using value_compare  = Compare;
    using allocator_type = Alloc;

private:
    using alloc_traits    = allocator_traits<Alloc>;
    using void_pointer    = typename alloc_traits::void_pointer;

public:
    using node_type          = rb_node<T, void_pointer>;
    using node_alloc_type    = typename alloc_traits::template rebind_alloc<node_type>;
    using node_alloc_traits  = allocator_traits<node_alloc_type>;
    // All pointer typedefs derive from the void pointer alone so that
    // instantiating rb_tree does not complete rb_node (incomplete-type support).
    using ptr_types          = rb_tree_ptr_types<T, void_pointer>;
    using node_pointer       = typename ptr_types::node_pointer;      // rb_node<T,VP>*
    using node_base_type     = rb_node_base<void_pointer>;
    using node_base_pointer  = typename ptr_types::node_base_ptr;     // rb_node_base<VP>*
    using end_node_type      = rb_end_node<node_base_pointer>;        // rb_end_node<nb_ptr>
    using end_node_pointer   = typename ptr_types::end_node_ptr;      // rb_end_node<nb_ptr>*

    using size_type          = typename alloc_traits::size_type;
    using difference_type    = typename alloc_traits::difference_type;

    using iterator           = rb_tree_iterator<T, void_pointer, difference_type>;
    using const_iterator     = rb_tree_const_iterator<T, void_pointer, difference_type>;

private:
    end_node_type    end_node_;    // sentinel
    end_node_pointer begin_node_;  // leftmost node or sentinel
    size_type        size_;
    value_compare    comp_;
    node_alloc_type  node_alloc_;

    end_node_pointer sentinel() noexcept {
        return pointer_traits<end_node_pointer>::pointer_to(end_node_);
    }
    end_node_pointer sentinel() const noexcept {
        return pointer_traits<end_node_pointer>::pointer_to(
            const_cast<end_node_type&>(end_node_));
    }

    // Root is end_node_.left (a node_base_pointer)
    node_base_pointer root_base() const noexcept {
        return end_node_.left;
    }
    node_pointer root() const noexcept {
        return rbtree_cast<node_pointer>(end_node_.left);
    }

    // ---- Node lifecycle ----
    template<class... Args>
    node_pointer alloc_node(Args&&... args) {
        node_pointer np = node_alloc_traits::allocate(node_alloc_, 1);
        node_alloc_traits::construct(node_alloc_, addressof(np->get_value()),
                                     std::forward<Args>(args)...);
        np->left = nullptr; np->right = nullptr;
        np->init_parent_red();   // parent = null, color = red (whole-word write)
        return np;
    }

    void free_node(node_pointer np) noexcept {
        node_alloc_traits::destroy(node_alloc_, addressof(np->get_value()));
        node_alloc_traits::deallocate(node_alloc_, np, 1);
    }

    void destroy_subtree(node_base_pointer np) noexcept {
        if (!np) return;
        destroy_subtree(np->left);
        node_base_pointer rc = np->right;
        free_node(rbtree_cast<node_pointer>(np));
        destroy_subtree(rc);
    }

    node_base_pointer copy_subtree(node_base_pointer src) {
        if (!src) return nullptr;
        node_pointer dn = alloc_node(rbtree_cast<node_pointer>(src)->get_value());
        dn->set_black(src->black());
        node_base_pointer lc = copy_subtree(src->left);
        node_base_pointer rc = copy_subtree(src->right);
        node_base_pointer dnb = rbtree_cast<node_base_pointer>(dn);
        dn->left = lc; dn->right = rc;
        if (lc) lc->set_parent(dnb);
        if (rc) rc->set_parent(dnb);
        return dnb;
    }

    void do_insert(end_node_pointer parent, node_base_pointer& child,
                   node_pointer np) noexcept
    {
        node_base_pointer npb = rbtree_cast<node_base_pointer>(np);
        np->set_parent_end(parent);
        np->left = nullptr; np->right = nullptr;
        child = npb;
        ++size_;
        if (begin_node_->left == npb) {  // np is new leftmost
            begin_node_ = rbtree_cast<end_node_pointer>(npb);
        }
        // balance — pass as node_base_ptr to the algorithm
        node_base_pointer r = root_base();
        tree_balance_after_insert(r, npb);
    }

    // find_unique: returns (parent, &child_slot)
    // *child_slot == nullptr means no duplicate
    template<class K>
    pair<end_node_pointer, node_base_pointer*> find_unique(const K& k) {
        end_node_pointer parent = sentinel();
        node_base_pointer* cp = &end_node_.left;
        node_base_pointer nd = root_base();
        while (nd) {
            node_pointer ndv = rbtree_cast<node_pointer>(nd);
            if (comp_(k, ndv->get_value())) {
                parent = rbtree_cast<end_node_pointer>(nd);
                cp = &nd->left;
                nd = nd->left;
            } else if (comp_(ndv->get_value(), k)) {
                parent = rbtree_cast<end_node_pointer>(nd);
                cp = &nd->right;
                nd = nd->right;
            } else {
                // equal
                parent = rbtree_cast<end_node_pointer>(nd);
                return {parent, cp};  // *cp points to this node (duplicate found)
            }
        }
        return {parent, cp};
    }

    template<class K>
    pair<end_node_pointer, node_base_pointer*> find_multi(const K& k) {
        end_node_pointer parent = sentinel();
        node_base_pointer* cp = &end_node_.left;
        node_base_pointer nd = root_base();
        while (nd) {
            parent = rbtree_cast<end_node_pointer>(nd);
            node_pointer ndv = rbtree_cast<node_pointer>(nd);
            if (comp_(k, ndv->get_value())) {
                cp = &nd->left;  nd = nd->left;
            } else {
                cp = &nd->right; nd = nd->right;
            }
        }
        return {parent, cp};
    }

    template<class K>
    pair<end_node_pointer, node_base_pointer*> find_unique_hint(const_iterator hint, const K& k) {
        if (hint != cend() && comp_(k, *hint)) {
            const_iterator prev = hint;
            if (prev == cbegin() || comp_(*--prev, k)) {
                node_base_pointer hn = (hint != cend()) ? hint.get_base() : nullptr;
                node_base_pointer pn = (hint != cbegin()) ? prev.get_base() : nullptr;
                if (pn && !pn->right)
                    return {rbtree_cast<end_node_pointer>(pn), &pn->right};
                else if (hn)
                    return {rbtree_cast<end_node_pointer>(hn), &hn->left};
            }
        }
        return find_unique(k);
    }

    // find_multi_low: leaf position BEFORE the equal range (libc++ __find_leaf_low)
    template<class K>
    pair<end_node_pointer, node_base_pointer*> find_multi_low(const K& k) {
        end_node_pointer parent = sentinel();
        node_base_pointer* cp = &end_node_.left;
        node_base_pointer nd = root_base();
        while (nd) {
            parent = rbtree_cast<end_node_pointer>(nd);
            node_pointer ndv = rbtree_cast<node_pointer>(nd);
            if (comp_(ndv->get_value(), k)) {
                cp = &nd->right; nd = nd->right;
            } else {
                cp = &nd->left;  nd = nd->left;
            }
        }
        return {parent, cp};
    }

    // libc++ __find_leaf: place as close to hint as possible
    template<class K>
    pair<end_node_pointer, node_base_pointer*> find_multi_hint(const_iterator hint, const K& k) {
        if (hint == cend() || !comp_(*hint, k)) {  // k <= *hint
            const_iterator prev = hint;
            if (prev == cbegin() || !comp_(k, *--prev)) {
                // *prev(hint) <= k <= *hint
                node_base_pointer hn = (hint != cend()) ? hint.get_base() : nullptr;
                node_base_pointer pn = (hint != cbegin()) ? prev.get_base() : nullptr;
                if (hn && !hn->left)
                    return {rbtree_cast<end_node_pointer>(hn), &hn->left};
                else if (pn)
                    return {rbtree_cast<end_node_pointer>(pn), &pn->right};
                // hint == cend() and tree empty
                return {sentinel(), &end_node_.left};
            }
            // k < *prev(hint): insert after the equal range
            return find_multi(k);
        }
        // k > *hint: insert before the equal range
        return find_multi_low(k);
    }

    void unlink(node_base_pointer np) noexcept {
        // update begin_node_ if np is leftmost
        if (rbtree_cast<node_base_pointer>(begin_node_) == np) {
            if (np->right)
                begin_node_ = rbtree_cast<end_node_pointer>(tree_min(rbtree_cast<node_base_pointer>(np->right)));
            else
                begin_node_ = np->get_parent_end();
        }
        node_base_pointer r = root_base();
        tree_remove(r, np);
        end_node_.left = r;
        --size_;
    }

    void steal(rb_tree& other) noexcept {
        end_node_.left = other.end_node_.left;
        if (root_base()) {
            root_base()->set_parent_end(sentinel());
            begin_node_ = other.begin_node_;
        } else {
            begin_node_ = sentinel();
        }
        size_ = other.size_;
        other.end_node_.left = nullptr;
        other.begin_node_ = other.sentinel();
        other.size_ = 0;
    }

public:
    const_iterator cbegin() const noexcept { return const_iterator(begin_node_); }
    const_iterator cend()   const noexcept { return const_iterator(sentinel()); }

    rb_tree() noexcept(noexcept(value_compare{}) && noexcept(node_alloc_type{}))
        : size_(0) { begin_node_ = sentinel(); }

    explicit rb_tree(const value_compare& c, const allocator_type& a = allocator_type())
        : size_(0), comp_(c), node_alloc_(a) { begin_node_ = sentinel(); }

    explicit rb_tree(const allocator_type& a)
        : size_(0), node_alloc_(a) { begin_node_ = sentinel(); }

    rb_tree(const rb_tree& o)
        : size_(0), comp_(o.comp_),
          node_alloc_(node_alloc_traits::select_on_container_copy_construction(o.node_alloc_))
    {
        begin_node_ = sentinel();
        if (!o.size_) return;
        end_node_.left = copy_subtree(o.root_base());
        root_base()->set_parent_end(sentinel());
        begin_node_ = rbtree_cast<end_node_pointer>(tree_min(root_base()));
        size_ = o.size_;
    }

    rb_tree(const rb_tree& o, const allocator_type& a)
        : size_(0), comp_(o.comp_), node_alloc_(a)
    {
        begin_node_ = sentinel();
        if (!o.size_) return;
        end_node_.left = copy_subtree(o.root_base());
        root_base()->set_parent_end(sentinel());
        begin_node_ = rbtree_cast<end_node_pointer>(tree_min(root_base()));
        size_ = o.size_;
    }

    rb_tree(rb_tree&& o) noexcept(
        is_nothrow_move_constructible_v<value_compare> &&
        is_nothrow_move_constructible_v<node_alloc_type>)
        : size_(0), comp_(move(o.comp_)), node_alloc_(move(o.node_alloc_))
    {
        begin_node_ = sentinel();
        if (o.size_) steal(o);
    }

    rb_tree(rb_tree&& o, const allocator_type& a)
        : size_(0), comp_(move(o.comp_)), node_alloc_(a)
    {
        begin_node_ = sentinel();
        if (!o.size_) return;
        if (node_alloc_ == o.node_alloc_) {
            steal(o);
        } else {
            for (auto& v : o) {
                auto [p, cp] = find_multi(v);
                do_insert(p, *cp, alloc_node(rb_value_mover<T>::do_move(v)));
            }
        }
    }

    ~rb_tree() { clear(); }

    rb_tree& operator=(const rb_tree& o) {
        if (this == addressof(o)) return *this;
        comp_ = o.comp_;
        if constexpr (node_alloc_traits::propagate_on_container_copy_assignment::value) {
            if (node_alloc_ != o.node_alloc_) clear();
            node_alloc_ = o.node_alloc_;
        }
        clear();
        if (!o.size_) return *this;
        end_node_.left = copy_subtree(o.root_base());
        root_base()->set_parent_end(sentinel());
        begin_node_ = rbtree_cast<end_node_pointer>(tree_min(root_base()));
        size_ = o.size_;
        return *this;
    }

    rb_tree& operator=(rb_tree&& o)
        noexcept(node_alloc_traits::propagate_on_container_move_assignment::value &&
                 is_nothrow_move_assignable_v<value_compare> &&
                 is_nothrow_move_assignable_v<node_alloc_type>)
    {
        if (this == addressof(o)) return *this;
        comp_ = move(o.comp_);
        if constexpr (node_alloc_traits::propagate_on_container_move_assignment::value) {
            clear(); node_alloc_ = move(o.node_alloc_); steal(o);
        } else if (node_alloc_ == o.node_alloc_) {
            clear(); steal(o);
        } else {
            clear();
            for (auto& v : o) {
                auto [p, cp] = find_multi(v);
                do_insert(p, *cp, alloc_node(rb_value_mover<T>::do_move(v)));
            }
        }
        return *this;
    }

    void swap(rb_tree& o) noexcept(is_nothrow_swappable_v<value_compare>) {
        using std::swap;
        swap(comp_, o.comp_);
        if constexpr (node_alloc_traits::propagate_on_container_swap::value)
            swap(node_alloc_, o.node_alloc_);
        swap(end_node_.left, o.end_node_.left);
        swap(begin_node_, o.begin_node_);
        swap(size_, o.size_);
        if (root_base()) root_base()->set_parent_end(sentinel());
        else begin_node_ = sentinel();
        if (o.root_base()) o.root_base()->set_parent_end(o.sentinel());
        else o.begin_node_ = o.sentinel();
        if (!size_) begin_node_ = sentinel();
        if (!o.size_) o.begin_node_ = o.sentinel();
    }

    size_type size()      const noexcept { return size_; }
    bool      empty()     const noexcept { return size_ == 0; }
    size_type max_size()  const noexcept {
        return min<size_type>(node_alloc_traits::max_size(node_alloc_),
                              numeric_limits<difference_type>::max());
    }
    allocator_type  get_allocator() const noexcept { return allocator_type(node_alloc_); }
    value_compare&       value_comp()       noexcept { return comp_; }
    const value_compare& value_comp() const noexcept { return comp_; }

    iterator       begin()       noexcept { return iterator(begin_node_); }
    const_iterator begin() const noexcept { return const_iterator(begin_node_); }
    iterator       end()         noexcept { return iterator(sentinel()); }
    const_iterator end()   const noexcept { return const_iterator(sentinel()); }

    void clear() noexcept {
        destroy_subtree(root_base());
        end_node_.left = nullptr;
        begin_node_ = sentinel();
        size_ = 0;
    }

    // ---- Unique emplace ----
    template<class... Args>
    pair<iterator, bool> emplace_unique(Args&&... args) {
        node_pointer np = alloc_node(std::forward<Args>(args)...);
        auto [p, cp] = find_unique(np->get_value());
        if (*cp != nullptr) {
            free_node(np);
            return {iterator(rbtree_cast<node_pointer>(*cp)), false};
        }
        do_insert(p, *cp, np);
        return {iterator(np), true};
    }

    template<class... Args>
    iterator emplace_hint_unique(const_iterator hint, Args&&... args) {
        node_pointer np = alloc_node(std::forward<Args>(args)...);
        auto [p, cp] = find_unique_hint(hint, np->get_value());
        if (*cp != nullptr) {
            free_node(np);
            return iterator(rbtree_cast<node_pointer>(*cp));
        }
        do_insert(p, *cp, np);
        return iterator(np);
    }

    // Search by an already-available key first; only allocate when absent
    // (allocator-requirements: duplicate insert must not allocate).
    template<class K, class... Args>
    pair<iterator, bool> emplace_unique_key_args(const K& k, Args&&... args) {
        auto [p, cp] = find_unique(k);
        if (*cp != nullptr)
            return {iterator(rbtree_cast<node_pointer>(*cp)), false};
        node_pointer np = alloc_node(std::forward<Args>(args)...);
        do_insert(p, *cp, np);
        return {iterator(np), true};
    }

    template<class K, class... Args>
    iterator emplace_hint_unique_key_args(const_iterator hint, const K& k, Args&&... args) {
        auto [p, cp] = find_unique_hint(hint, k);
        if (*cp != nullptr)
            return iterator(rbtree_cast<node_pointer>(*cp));
        node_pointer np = alloc_node(std::forward<Args>(args)...);
        do_insert(p, *cp, np);
        return iterator(np);
    }

    template<class K, class... Args>
    pair<iterator, bool> try_emplace_unique(K&& k, Args&&... args) {
        auto [p, cp] = find_unique(k);
        if (*cp != nullptr)
            return {iterator(rbtree_cast<node_pointer>(*cp)), false};
        node_pointer np = alloc_node(piecewise_construct,
                                     forward_as_tuple(std::forward<K>(k)),
                                     forward_as_tuple(std::forward<Args>(args)...));
        do_insert(p, *cp, np);
        return {iterator(np), true};
    }

    template<class K, class... Args>
    iterator try_emplace_hint_unique(const_iterator hint, K&& k, Args&&... args) {
        auto [p, cp] = find_unique_hint(hint, k);
        if (*cp != nullptr)
            return iterator(rbtree_cast<node_pointer>(*cp));
        node_pointer np = alloc_node(piecewise_construct,
                                     forward_as_tuple(std::forward<K>(k)),
                                     forward_as_tuple(std::forward<Args>(args)...));
        do_insert(p, *cp, np);
        return iterator(np);
    }

    template<class K, class M>
    pair<iterator, bool> insert_or_assign(K&& k, M&& obj) {
        auto [p, cp] = find_unique(k);
        if (*cp != nullptr) {
            node_pointer ex = rbtree_cast<node_pointer>(*cp);
            ex->get_value().second = std::forward<M>(obj);
            return {iterator(ex), false};
        }
        node_pointer np = alloc_node(piecewise_construct,
                                     forward_as_tuple(std::forward<K>(k)),
                                     forward_as_tuple(std::forward<M>(obj)));
        do_insert(p, *cp, np);
        return {iterator(np), true};
    }

    template<class K, class M>
    iterator insert_or_assign_hint(const_iterator hint, K&& k, M&& obj) {
        auto [p, cp] = find_unique_hint(hint, k);
        if (*cp != nullptr) {
            node_pointer ex = rbtree_cast<node_pointer>(*cp);
            ex->get_value().second = std::forward<M>(obj);
            return iterator(ex);
        }
        node_pointer np = alloc_node(piecewise_construct,
                                     forward_as_tuple(std::forward<K>(k)),
                                     forward_as_tuple(std::forward<M>(obj)));
        do_insert(p, *cp, np);
        return iterator(np);
    }

    // ---- Multi emplace ----
    template<class... Args>
    iterator emplace_multi(Args&&... args) {
        node_pointer np = alloc_node(std::forward<Args>(args)...);
        auto [p, cp] = find_multi(np->get_value());
        do_insert(p, *cp, np);
        return iterator(np);
    }

    template<class... Args>
    iterator emplace_hint_multi(const_iterator hint, Args&&... args) {
        node_pointer np = alloc_node(std::forward<Args>(args)...);
        auto [p, cp] = find_multi_hint(hint, np->get_value());
        do_insert(p, *cp, np);
        return iterator(np);
    }

    // ---- Erase ----
    iterator erase(const_iterator pos) noexcept {
        node_base_pointer np = pos.get_base();
        const_iterator nx = pos; ++nx;
        unlink(np);
        free_node(rbtree_cast<node_pointer>(np));
        return iterator(nx.ptr_);
    }

    iterator erase(const_iterator first, const_iterator last) noexcept {
        while (first != last)
            first = const_iterator(erase(first).ptr_);
        return iterator(last.ptr_);
    }

    template<class K>
    size_type erase_unique(const K& k) {
        auto it = find(k);
        if (it == end()) return 0;
        erase(it); return 1;
    }

    template<class K>
    size_type erase_multi(const K& k) {
        auto [lo, hi] = equal_range_multi(k);
        size_type n = 0;
        while (lo != hi) { lo = erase(lo); ++n; }
        return n;
    }

    // ---- Lookup ----
    // find returns the FIRST element equivalent to k (matters for multi
    // containers, where any-equal would be wrong).
    template<class K>
    iterator find(const K& k) {
        iterator it = lower_bound(k);
        if (it != end() && !comp_(k, *it)) return it;
        return end();
    }

    template<class K>
    const_iterator find(const K& k) const {
        return const_cast<rb_tree*>(this)->find(k);
    }

    template<class K>
    size_type count_unique(const K& k) const { return find(k) != end() ? 1 : 0; }

    template<class K>
    size_type count_multi(const K& k) const {
        auto [lo, hi] = equal_range_multi(k);
        size_type n = 0;
        for (auto it = lo; it != hi; ++it) ++n;
        return n;
    }

    template<class K>
    iterator lower_bound(const K& k) {
        end_node_pointer result = sentinel();
        node_base_pointer nd = root_base();
        while (nd) {
            if (!comp_(rbtree_cast<node_pointer>(nd)->get_value(), k)) {
                result = rbtree_cast<end_node_pointer>(nd);
                nd = nd->left;
            } else {
                nd = nd->right;
            }
        }
        return iterator(result);
    }

    template<class K>
    const_iterator lower_bound(const K& k) const {
        return const_cast<rb_tree*>(this)->lower_bound(k);
    }

    template<class K>
    iterator upper_bound(const K& k) {
        end_node_pointer result = sentinel();
        node_base_pointer nd = root_base();
        while (nd) {
            if (comp_(k, rbtree_cast<node_pointer>(nd)->get_value())) {
                result = rbtree_cast<end_node_pointer>(nd);
                nd = nd->left;
            } else {
                nd = nd->right;
            }
        }
        return iterator(result);
    }

    template<class K>
    const_iterator upper_bound(const K& k) const {
        return const_cast<rb_tree*>(this)->upper_bound(k);
    }

    template<class K>
    pair<iterator, iterator> equal_range_unique(const K& k) {
        return {lower_bound(k), upper_bound(k)};
    }
    template<class K>
    pair<const_iterator, const_iterator> equal_range_unique(const K& k) const {
        return {lower_bound(k), upper_bound(k)};
    }
    template<class K>
    pair<iterator, iterator> equal_range_multi(const K& k) {
        return {lower_bound(k), upper_bound(k)};
    }
    template<class K>
    pair<const_iterator, const_iterator> equal_range_multi(const K& k) const {
        return {lower_bound(k), upper_bound(k)};
    }

    // ---- Node handle ----
    node_pointer extract_node(const_iterator pos) noexcept {
        node_base_pointer np = pos.get_base();
        unlink(np);
        np->left = nullptr; np->right = nullptr; np->init_parent_red();
        return rbtree_cast<node_pointer>(np);
    }

    template<class K>
    node_pointer extract_node(const K& k) noexcept {
        iterator it = find(k);
        if (it == end()) return nullptr;
        return extract_node(const_iterator(it.ptr_));
    }

    pair<iterator, bool> insert_node_unique(node_pointer np) {
        auto [p, cp] = find_unique(np->get_value());
        if (*cp) return {iterator(rbtree_cast<node_pointer>(*cp)), false};
        do_insert(p, *cp, np);
        return {iterator(np), true};
    }

    iterator insert_node_multi(node_pointer np) {
        auto [p, cp] = find_multi(np->get_value());
        do_insert(p, *cp, np);
        return iterator(np);
    }

    template<class Comp2>
    void merge_unique(rb_tree<T, Comp2, Alloc>& src) {
        auto it = src.begin();
        while (it != src.end()) {
            auto nx = it; ++nx;
            auto [p, cp] = find_unique(*it);
            if (!*cp) {
                node_pointer np = src.extract_node(const_iterator(it.ptr_));
                do_insert(p, *cp, np);
            }
            it = nx;
        }
    }

    template<class Comp2>
    void merge_multi(rb_tree<T, Comp2, Alloc>& src) {
        while (!src.empty()) {
            // Use cbegin() to force const_iterator overload of extract_node,
            // avoiding ambiguity with the template overload.
            node_pointer np = src.extract_node(src.cbegin());
            auto [p, cp] = find_multi(np->get_value());
            do_insert(p, *cp, np);
        }
    }

    node_alloc_type& get_node_alloc() noexcept { return node_alloc_; }
};

} // namespace std
