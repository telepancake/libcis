// lean/include/bits/lean_tree.h — red-black tree for the lean map/set profile.
//
// Representation (the lean contract):
//   node header  { uintptr_t parent_and_color; tree_node_base* left, *right; }
//   color is BIT 0 of parent_and_color (nodes are malloc-aligned -> bit 0 free;
//   bit0 set == red, clear == black). The stored value follows the header in the
//   SAME malloc allocation, at offset align_up(sizeof(header), alignof(T)).
//   One malloc per node, no allocator.
//
// The container embeds a header node (this is rb_tree::header_) that doubles as
// the end() sentinel, exactly like libstdc++:
//   header.parent = root, header.left = leftmost (begin, O(1)),
//   header.right = rightmost (--end(), O(1)); the header is kept RED so the
//   decrement algorithm can recognise it. size_ counts nodes.
//
// Type-erasure split (the one libstdc++ ships in its .so): every type-independent
// structural routine — rotate, insert-rebalance, erase-rebalance/fixup,
// increment, decrement, minimum, maximum, and the destroy-walk (which takes a
// per-type destroy callback) — is a NON-TEMPLATE inline function operating on
// tree_node_base. Only the comparator descent loops and node create/destroy stay
// per-type (thin) inside rb_tree.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>
#include <limits>
#include <type_traits>
#include <utility>
#include <tuple>
#include <iterator>
#include <bits/lean.h>   // std::detail::lean_allocator_ok, std::allocator fwd

namespace std {

namespace detail {

// libc++ __is_allocator, needed by the map/set deduction guides. Defined here
// because the lean overlay replaces bits/tree.h wholesale (the two never share a
// translation unit).
template<class Alloc>
inline constexpr bool tree_is_allocator_v = requires(Alloc& a) {
    typename Alloc::value_type;
    a.allocate(size_t{});
};

// ---------------------------------------------------------------------------
// Node header + bit-0 color accessors (type-independent).
// ---------------------------------------------------------------------------
struct tree_node_base {
    uintptr_t       parent_and_color;
    tree_node_base* left;
    tree_node_base* right;
};

inline tree_node_base* tree_parent(const tree_node_base* n) noexcept {
    return reinterpret_cast<tree_node_base*>(n->parent_and_color & ~uintptr_t(1));
}
inline void tree_set_parent(tree_node_base* n, tree_node_base* p) noexcept {
    n->parent_and_color =
        reinterpret_cast<uintptr_t>(p) | (n->parent_and_color & uintptr_t(1));
}
inline bool tree_is_red(const tree_node_base* n) noexcept {
    return n->parent_and_color & uintptr_t(1);
}
inline void tree_set_red(tree_node_base* n) noexcept {
    n->parent_and_color |= uintptr_t(1);
}
inline void tree_set_black(tree_node_base* n) noexcept {
    n->parent_and_color &= ~uintptr_t(1);
}
inline void tree_set_color(tree_node_base* n, bool red) noexcept {
    if (red) tree_set_red(n); else tree_set_black(n);
}

// root is stored in the header's (packed) parent field.
inline tree_node_base* tree_root(tree_node_base* header) noexcept {
    return tree_parent(header);
}
inline void tree_set_root(tree_node_base* header, tree_node_base* r) noexcept {
    tree_set_parent(header, r);
}

inline tree_node_base* tree_min(tree_node_base* x) noexcept {
    while (x->left) x = x->left;
    return x;
}
inline tree_node_base* tree_max(tree_node_base* x) noexcept {
    while (x->right) x = x->right;
    return x;
}

// In-order successor / predecessor (libstdc++ _Rb_tree_increment/decrement).
// The header is recognised in decrement by: red AND parent->parent == self
// (the header<->root two-cycle). root is always black, so no ordinary node
// matches.
inline tree_node_base* tree_increment(tree_node_base* x) noexcept {
    if (x->right) {
        x = x->right;
        while (x->left) x = x->left;
    } else {
        tree_node_base* y = tree_parent(x);
        while (x == y->right) { x = y; y = tree_parent(y); }
        if (x->right != y) x = y;
    }
    return x;
}
inline tree_node_base* tree_decrement(tree_node_base* x) noexcept {
    if (tree_is_red(x) && tree_parent(tree_parent(x)) == x) {
        x = x->right;                 // x is the header -> rightmost
    } else if (x->left) {
        tree_node_base* y = x->left;
        while (y->right) y = y->right;
        x = y;
    } else {
        tree_node_base* y = tree_parent(x);
        while (x == y->left) { x = y; y = tree_parent(y); }
        x = y;
    }
    return x;
}

inline void tree_rotate_left(tree_node_base* x, tree_node_base* header) noexcept {
    tree_node_base* y  = x->right;
    tree_node_base* xp = tree_parent(x);
    x->right = y->left;
    if (y->left) tree_set_parent(y->left, x);
    tree_set_parent(y, xp);
    if (x == tree_root(header)) tree_set_root(header, y);
    else if (x == xp->left)     xp->left = y;
    else                        xp->right = y;
    y->left = x;
    tree_set_parent(x, y);
}
inline void tree_rotate_right(tree_node_base* x, tree_node_base* header) noexcept {
    tree_node_base* y  = x->left;
    tree_node_base* xp = tree_parent(x);
    x->left = y->right;
    if (y->right) tree_set_parent(y->right, x);
    tree_set_parent(y, xp);
    if (x == tree_root(header)) tree_set_root(header, y);
    else if (x == xp->right)    xp->right = y;
    else                        xp->left = y;
    y->right = x;
    tree_set_parent(x, y);
}

// Link a fresh node x under parent p (as left/right per insert_left), then
// restore the red-black invariants. Maintains header.left/right (leftmost /
// rightmost). Port of libstdc++ _Rb_tree_insert_and_rebalance.
inline void tree_insert_and_rebalance(bool insert_left, tree_node_base* x,
                                      tree_node_base* p,
                                      tree_node_base* header) noexcept {
    x->parent_and_color = reinterpret_cast<uintptr_t>(p) | uintptr_t(1); // parent=p, red
    x->left = nullptr;
    x->right = nullptr;

    if (insert_left) {
        p->left = x;
        if (p == header) { tree_set_root(header, x); header->right = x; }
        else if (p == header->left) header->left = x;
    } else {
        p->right = x;
        if (p == header->right) header->right = x;
    }

    while (x != tree_root(header) && tree_is_red(tree_parent(x))) {
        tree_node_base* xp  = tree_parent(x);
        tree_node_base* xpp = tree_parent(xp);
        if (xp == xpp->left) {
            tree_node_base* y = xpp->right;
            if (y && tree_is_red(y)) {
                tree_set_black(xp);
                tree_set_black(y);
                tree_set_red(xpp);
                x = xpp;
            } else {
                if (x == xp->right) {
                    x = xp;
                    tree_rotate_left(x, header);
                    xp = tree_parent(x);
                }
                tree_set_black(xp);
                tree_set_red(xpp);
                tree_rotate_right(xpp, header);
            }
        } else {
            tree_node_base* y = xpp->left;
            if (y && tree_is_red(y)) {
                tree_set_black(xp);
                tree_set_black(y);
                tree_set_red(xpp);
                x = xpp;
            } else {
                if (x == xp->left) {
                    x = xp;
                    tree_rotate_right(x, header);
                    xp = tree_parent(x);
                }
                tree_set_black(xp);
                tree_set_red(xpp);
                tree_rotate_left(xpp, header);
            }
        }
    }
    tree_set_black(tree_root(header));
}

// Unlink z and repair the tree. Returns the node that must be physically freed
// (always z: when z has two children the successor is relinked into z's slot so
// value addresses of surviving elements never move -> reference stability).
// Port of libstdc++ _Rb_tree_rebalance_for_erase.
inline tree_node_base* tree_rebalance_for_erase(tree_node_base* z,
                                                tree_node_base* header) noexcept {
    tree_node_base* y = z;
    tree_node_base* x = nullptr;
    tree_node_base* x_parent = nullptr;

    if (y->left == nullptr)        x = y->right;
    else if (y->right == nullptr)  x = y->left;
    else {
        y = y->right;
        while (y->left) y = y->left;
        x = y->right;
    }

    if (y != z) {
        // relink y (z's successor) into z's structural position.
        tree_set_parent(z->left, y);
        y->left = z->left;
        if (y != z->right) {
            tree_node_base* yp = tree_parent(y);   // y is a left child
            x_parent = yp;
            if (x) tree_set_parent(x, yp);
            yp->left = x;
            y->right = z->right;
            tree_set_parent(z->right, y);
        } else {
            x_parent = y;
        }
        tree_node_base* zp = tree_parent(z);
        if (tree_root(header) == z)   tree_set_root(header, y);
        else if (zp->left == z)       zp->left = y;
        else                          zp->right = y;
        tree_set_parent(y, zp);
        bool cy = tree_is_red(y), cz = tree_is_red(z);
        tree_set_color(y, cz);
        tree_set_color(z, cy);
        y = z;   // node to physically delete
    } else {
        // y == z: at most one child.
        tree_node_base* zp = tree_parent(z);
        x_parent = zp;
        if (x) tree_set_parent(x, zp);
        if (tree_root(header) == z)   tree_set_root(header, x);
        else if (zp->left == z)       zp->left = x;
        else                          zp->right = x;
        if (header->left == z) {
            if (z->right == nullptr) header->left = zp;      // z had no children
            else                     header->left = tree_min(x);
        }
        if (header->right == z) {
            if (z->left == nullptr)  header->right = zp;
            else                     header->right = tree_max(x);
        }
    }

    if (!tree_is_red(y)) {  // a black node was removed -> fix black-height
        while (x != tree_root(header) && (x == nullptr || !tree_is_red(x))) {
            if (x == x_parent->left) {
                tree_node_base* w = x_parent->right;
                if (tree_is_red(w)) {
                    tree_set_black(w);
                    tree_set_red(x_parent);
                    tree_rotate_left(x_parent, header);
                    w = x_parent->right;
                }
                if ((w->left  == nullptr || !tree_is_red(w->left)) &&
                    (w->right == nullptr || !tree_is_red(w->right))) {
                    tree_set_red(w);
                    x = x_parent;
                    x_parent = tree_parent(x_parent);
                } else {
                    if (w->right == nullptr || !tree_is_red(w->right)) {
                        tree_set_black(w->left);
                        tree_set_red(w);
                        tree_rotate_right(w, header);
                        w = x_parent->right;
                    }
                    tree_set_color(w, tree_is_red(x_parent));
                    tree_set_black(x_parent);
                    if (w->right) tree_set_black(w->right);
                    tree_rotate_left(x_parent, header);
                    break;
                }
            } else {
                tree_node_base* w = x_parent->left;
                if (tree_is_red(w)) {
                    tree_set_black(w);
                    tree_set_red(x_parent);
                    tree_rotate_right(x_parent, header);
                    w = x_parent->left;
                }
                if ((w->right == nullptr || !tree_is_red(w->right)) &&
                    (w->left  == nullptr || !tree_is_red(w->left))) {
                    tree_set_red(w);
                    x = x_parent;
                    x_parent = tree_parent(x_parent);
                } else {
                    if (w->left == nullptr || !tree_is_red(w->left)) {
                        tree_set_black(w->right);
                        tree_set_red(w);
                        tree_rotate_left(w, header);
                        w = x_parent->left;
                    }
                    tree_set_color(w, tree_is_red(x_parent));
                    tree_set_black(x_parent);
                    if (w->left) tree_set_black(w->left);
                    tree_rotate_right(x_parent, header);
                    break;
                }
            }
        }
        if (x) tree_set_black(x);
    }
    return y;
}

// Type-independent post-order destroy walk. The per-type callback ends the
// value's lifetime and frees the node.
inline void tree_destroy(tree_node_base* n,
                         void (*destroy_node)(tree_node_base*)) noexcept {
    while (n) {
        tree_destroy(n->left, destroy_node);
        tree_node_base* r = n->right;
        destroy_node(n);
        n = r;
    }
}

// Value payload offset inside a node allocation.
inline constexpr size_t tree_align_up(size_t n, size_t a) noexcept {
    return (n + a - 1) & ~(a - 1);
}
template<class T>
inline constexpr size_t tree_value_offset =
    tree_align_up(sizeof(tree_node_base), alignof(T));

template<class T>
inline T* tree_value_ptr(tree_node_base* n) noexcept {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(n) + tree_value_offset<T>);
}
template<class T>
inline const T* tree_value_ptr(const tree_node_base* n) noexcept {
    return reinterpret_cast<const T*>(
        reinterpret_cast<const char*>(n) + tree_value_offset<T>);
}

} // namespace detail

// ============================================================
// Iterators (thin per-type wrappers over tree_node_base*).
// ============================================================
template<class T> class rb_const_iterator;

template<class T>
class rb_iterator {
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using reference         = T&;
    using pointer           = T*;

private:
    detail::tree_node_base* node_;
    friend class rb_const_iterator<T>;
    template<class, class, class> friend class rb_tree;

public:
    rb_iterator() noexcept : node_(nullptr) {}
    explicit rb_iterator(detail::tree_node_base* n) noexcept : node_(n) {}

    reference operator*()  const noexcept { return *detail::tree_value_ptr<T>(node_); }
    pointer   operator->() const noexcept { return detail::tree_value_ptr<T>(node_); }

    rb_iterator& operator++() noexcept { node_ = detail::tree_increment(node_); return *this; }
    rb_iterator  operator++(int) noexcept { rb_iterator t = *this; ++*this; return t; }
    rb_iterator& operator--() noexcept { node_ = detail::tree_decrement(node_); return *this; }
    rb_iterator  operator--(int) noexcept { rb_iterator t = *this; --*this; return t; }

    friend bool operator==(const rb_iterator& a, const rb_iterator& b) noexcept {
        return a.node_ == b.node_;
    }
    friend bool operator!=(const rb_iterator& a, const rb_iterator& b) noexcept {
        return a.node_ != b.node_;
    }
};

template<class T>
class rb_const_iterator {
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using reference         = const T&;
    using pointer           = const T*;

private:
    detail::tree_node_base* node_;
    template<class, class, class> friend class rb_tree;

public:
    rb_const_iterator() noexcept : node_(nullptr) {}
    explicit rb_const_iterator(detail::tree_node_base* n) noexcept : node_(n) {}
    rb_const_iterator(const rb_iterator<T>& it) noexcept : node_(it.node_) {}

    reference operator*()  const noexcept { return *detail::tree_value_ptr<T>(node_); }
    pointer   operator->() const noexcept { return detail::tree_value_ptr<T>(node_); }

    rb_const_iterator& operator++() noexcept { node_ = detail::tree_increment(node_); return *this; }
    rb_const_iterator  operator++(int) noexcept { rb_const_iterator t = *this; ++*this; return t; }
    rb_const_iterator& operator--() noexcept { node_ = detail::tree_decrement(node_); return *this; }
    rb_const_iterator  operator--(int) noexcept { rb_const_iterator t = *this; --*this; return t; }

    friend bool operator==(const rb_const_iterator& a, const rb_const_iterator& b) noexcept {
        return a.node_ == b.node_;
    }
    friend bool operator!=(const rb_const_iterator& a, const rb_const_iterator& b) noexcept {
        return a.node_ != b.node_;
    }
};

// ============================================================
// rb_tree
//
// Alloc is retained only for source compatibility with the base map/set
// headers; the lean profile allocates every node with malloc/free (no
// allocator). map/set enforce the allocator == std::allocator<T> contract with
// their own static_assert (std::detail::lean_allocator_ok).
// ============================================================
template<class T, class Compare, class Alloc = allocator<T>>
class rb_tree {
public:
    using value_type     = T;
    using value_compare  = Compare;
    using allocator_type = Alloc;
    using size_type      = size_t;
    using difference_type = ptrdiff_t;
    using iterator       = rb_iterator<T>;
    using const_iterator = rb_const_iterator<T>;

private:
    using node_base = detail::tree_node_base;

    node_base                     header_;
    size_t                        size_;
    [[no_unique_address]] Compare comp_;

    node_base* header_ptr() const noexcept {
        return const_cast<node_base*>(&header_);
    }
    node_base* root_node() const noexcept {
        return detail::tree_parent(&header_);
    }
    static T& value_of(node_base* n) noexcept {
        return *detail::tree_value_ptr<T>(n);
    }

    void init_empty() noexcept {
        header_.parent_and_color = uintptr_t(1);   // parent=null, header kept RED
        header_.left  = &header_;
        header_.right = &header_;
    }

    template<class... Args>
    node_base* create_node(Args&&... args) {
        void* mem = ::malloc(detail::tree_value_offset<T> + sizeof(T));
        if (!mem) __builtin_trap();
        node_base* n = static_cast<node_base*>(mem);
        ::new (static_cast<void*>(detail::tree_value_ptr<T>(n)))
            T(std::forward<Args>(args)...);
        return n;
    }
    static void destroy_node_cb(node_base* n) noexcept {
        detail::tree_value_ptr<T>(n)->~T();
        ::free(n);
    }
    void destroy_node(node_base* n) noexcept { destroy_node_cb(n); }

    // Recursive per-type clone (values copied; structure and colors preserved).
    node_base* clone_subtree(node_base* src, node_base* parent) {
        node_base* n = create_node(value_of(src));
        n->parent_and_color =
            reinterpret_cast<uintptr_t>(parent) | (detail::tree_is_red(src) ? 1u : 0u);
        n->left  = src->left  ? clone_subtree(src->left,  n) : nullptr;
        n->right = src->right ? clone_subtree(src->right, n) : nullptr;
        return n;
    }
    void copy_from(const rb_tree& o) {
        node_base* r = clone_subtree(o.root_node(), &header_);
        header_.parent_and_color = reinterpret_cast<uintptr_t>(r) | uintptr_t(1);
        header_.left  = detail::tree_min(r);
        header_.right = detail::tree_max(r);
        size_ = o.size_;
    }
    void steal(rb_tree& o) noexcept {
        node_base* r = o.root_node();
        header_.parent_and_color = reinterpret_cast<uintptr_t>(r) | uintptr_t(1);
        header_.left  = o.header_.left;
        header_.right = o.header_.right;
        detail::tree_set_parent(r, &header_);
        size_ = o.size_;
        o.init_empty();
        o.size_ = 0;
    }

    // ---- descent (thin, per-type) ----
    struct insert_pos { node_base* parent; bool left; };

    template<class K>
    node_base* find_unique_pos(const K& k, insert_pos& pos) {
        node_base* x = root_node();
        node_base* p = &header_;
        bool left = true;
        while (x) {
            p = x;
            if (comp_(k, value_of(x)))       { left = true;  x = x->left; }
            else if (comp_(value_of(x), k))  { left = false; x = x->right; }
            else                             { return x; }   // duplicate
        }
        pos.parent = p; pos.left = left;
        return nullptr;
    }

    template<class K>
    void find_multi_pos(const K& k, insert_pos& pos) {
        node_base* x = root_node();
        node_base* p = &header_;
        bool left = true;
        while (x) {
            p = x;
            if (comp_(k, value_of(x))) { left = true;  x = x->left; }
            else                       { left = false; x = x->right; }
        }
        pos.parent = p; pos.left = left;
    }

    // Hint variants: cheap check of the "just before hint" slot, else full
    // search (libc++ __find_leaf_high/__find_leaf semantics).
    template<class K>
    node_base* find_unique_hint_pos(const_iterator hint, const K& k, insert_pos& pos) {
        node_base* hn = hint.node_;
        if (hn != &header_ && comp_(k, value_of(hn))) {           // k < *hint
            if (hint == cbegin()) { pos.parent = hn; pos.left = true; return nullptr; }
            const_iterator prev = hint; --prev;
            node_base* pn = prev.node_;
            if (comp_(value_of(pn), k)) {                          // *prev < k < *hint
                if (pn->right == nullptr) { pos.parent = pn; pos.left = false; return nullptr; }
                pos.parent = hn; pos.left = true; return nullptr;
            }
        }
        return find_unique_pos(k, pos);
    }

    template<class K>
    void find_multi_hint_pos(const_iterator hint, const K& k, insert_pos& pos) {
        node_base* hn = hint.node_;
        if (hn == &header_ || !comp_(value_of(hn), k)) {           // k <= *hint
            if (hint == cbegin()) {
                if (hn != &header_) { pos.parent = hn; pos.left = true; return; }
                // empty tree: insert at header
                pos.parent = &header_; pos.left = true; return;
            }
            const_iterator prev = hint; --prev;
            node_base* pn = prev.node_;
            if (!comp_(k, value_of(pn))) {                         // *prev <= k <= *hint
                if (hn != &header_ && hn->left == nullptr) { pos.parent = hn; pos.left = true; return; }
                pos.parent = pn; pos.left = false; return;
            }
            find_multi_pos(k, pos);   // k < *prev
            return;
        }
        find_multi_low_pos(k, pos);   // k > *hint
    }

    // Leaf position at the low end of the equal range (libc++ __find_leaf_low).
    template<class K>
    void find_multi_low_pos(const K& k, insert_pos& pos) {
        node_base* x = root_node();
        node_base* p = &header_;
        bool left = true;
        while (x) {
            p = x;
            if (comp_(value_of(x), k)) { left = false; x = x->right; }
            else                       { left = true;  x = x->left; }
        }
        pos.parent = p; pos.left = left;
    }

    iterator insert_at(insert_pos pos, node_base* z) {
        detail::tree_insert_and_rebalance(pos.left, z, pos.parent, &header_);
        ++size_;
        return iterator(z);
    }

public:
    const_iterator cbegin() const noexcept { return const_iterator(header_.left); }
    const_iterator cend()   const noexcept { return const_iterator(header_ptr()); }

    rb_tree() noexcept(noexcept(Compare())) : size_(0), comp_() { init_empty(); }

    explicit rb_tree(const value_compare& c, const allocator_type& = allocator_type())
        : size_(0), comp_(c) { init_empty(); }

    explicit rb_tree(const allocator_type&) : size_(0), comp_() { init_empty(); }

    rb_tree(const rb_tree& o) : size_(0), comp_(o.comp_) {
        init_empty();
        if (o.size_) copy_from(o);
    }
    rb_tree(const rb_tree& o, const allocator_type&) : size_(0), comp_(o.comp_) {
        init_empty();
        if (o.size_) copy_from(o);
    }
    rb_tree(rb_tree&& o) noexcept(is_nothrow_move_constructible_v<Compare>)
        : size_(0), comp_(move(o.comp_)) {
        init_empty();
        if (o.size_) steal(o);
    }
    rb_tree(rb_tree&& o, const allocator_type&) : size_(0), comp_(move(o.comp_)) {
        init_empty();
        if (o.size_) steal(o);
    }

    ~rb_tree() { clear(); }

    rb_tree& operator=(const rb_tree& o) {
        if (this == &o) return *this;
        clear();
        comp_ = o.comp_;
        if (o.size_) copy_from(o);
        return *this;
    }
    rb_tree& operator=(rb_tree&& o)
        noexcept(is_nothrow_move_assignable_v<Compare>) {
        if (this == &o) return *this;
        clear();
        comp_ = move(o.comp_);
        if (o.size_) steal(o);
        return *this;
    }

    void swap(rb_tree& o) noexcept(is_nothrow_swappable_v<Compare>) {
        using std::swap;
        swap(comp_, o.comp_);
        node_base* r1 = root_node();
        node_base* r2 = o.root_node();
        swap(size_, o.size_);
        swap(header_.left,  o.header_.left);
        swap(header_.right, o.header_.right);
        header_.parent_and_color   = reinterpret_cast<uintptr_t>(r2) | uintptr_t(1);
        o.header_.parent_and_color = reinterpret_cast<uintptr_t>(r1) | uintptr_t(1);
        if (root_node()) detail::tree_set_parent(root_node(), &header_);
        else { header_.left = &header_; header_.right = &header_; }
        if (o.root_node()) detail::tree_set_parent(o.root_node(), &o.header_);
        else { o.header_.left = &o.header_; o.header_.right = &o.header_; }
    }

    allocator_type get_allocator() const noexcept { return allocator_type(); }
    value_compare&       value_comp()       noexcept { return comp_; }
    const value_compare& value_comp() const noexcept { return comp_; }

    iterator       begin()       noexcept { return iterator(header_.left); }
    const_iterator begin() const noexcept { return const_iterator(header_.left); }
    iterator       end()         noexcept { return iterator(header_ptr()); }
    const_iterator end()   const noexcept { return const_iterator(header_ptr()); }

    bool      empty()    const noexcept { return size_ == 0; }
    size_type size()     const noexcept { return size_; }
    size_type max_size() const noexcept {
        return numeric_limits<ptrdiff_t>::max() /
               static_cast<ptrdiff_t>(detail::tree_value_offset<T> + sizeof(T));
    }

    void clear() noexcept {
        detail::tree_destroy(root_node(), &destroy_node_cb);
        init_empty();
        size_ = 0;
    }

    // ---- Unique emplace ----
    template<class... Args>
    pair<iterator, bool> emplace_unique(Args&&... args) {
        node_base* z = create_node(std::forward<Args>(args)...);
        insert_pos pos;
        node_base* dup = find_unique_pos(value_of(z), pos);
        if (dup) { destroy_node(z); return {iterator(dup), false}; }
        return {insert_at(pos, z), true};
    }
    template<class... Args>
    iterator emplace_hint_unique(const_iterator hint, Args&&... args) {
        node_base* z = create_node(std::forward<Args>(args)...);
        insert_pos pos;
        node_base* dup = find_unique_hint_pos(hint, value_of(z), pos);
        if (dup) { destroy_node(z); return iterator(dup); }
        return insert_at(pos, z);
    }
    template<class K, class... Args>
    pair<iterator, bool> emplace_unique_key_args(const K& k, Args&&... args) {
        insert_pos pos;
        node_base* dup = find_unique_pos(k, pos);
        if (dup) return {iterator(dup), false};
        node_base* z = create_node(std::forward<Args>(args)...);
        return {insert_at(pos, z), true};
    }
    template<class K, class... Args>
    iterator emplace_hint_unique_key_args(const_iterator hint, const K& k, Args&&... args) {
        insert_pos pos;
        node_base* dup = find_unique_hint_pos(hint, k, pos);
        if (dup) return iterator(dup);
        node_base* z = create_node(std::forward<Args>(args)...);
        return insert_at(pos, z);
    }
    template<class K, class... Args>
    pair<iterator, bool> try_emplace_unique(K&& k, Args&&... args) {
        insert_pos pos;
        node_base* dup = find_unique_pos(k, pos);
        if (dup) return {iterator(dup), false};
        node_base* z = create_node(piecewise_construct,
                                   forward_as_tuple(std::forward<K>(k)),
                                   forward_as_tuple(std::forward<Args>(args)...));
        return {insert_at(pos, z), true};
    }
    template<class K, class... Args>
    iterator try_emplace_hint_unique(const_iterator hint, K&& k, Args&&... args) {
        insert_pos pos;
        node_base* dup = find_unique_hint_pos(hint, k, pos);
        if (dup) return iterator(dup);
        node_base* z = create_node(piecewise_construct,
                                   forward_as_tuple(std::forward<K>(k)),
                                   forward_as_tuple(std::forward<Args>(args)...));
        return insert_at(pos, z);
    }
    template<class K, class M>
    pair<iterator, bool> insert_or_assign(K&& k, M&& obj) {
        insert_pos pos;
        node_base* dup = find_unique_pos(k, pos);
        if (dup) {
            value_of(dup).second = std::forward<M>(obj);
            return {iterator(dup), false};
        }
        node_base* z = create_node(piecewise_construct,
                                   forward_as_tuple(std::forward<K>(k)),
                                   forward_as_tuple(std::forward<M>(obj)));
        return {insert_at(pos, z), true};
    }
    template<class K, class M>
    iterator insert_or_assign_hint(const_iterator hint, K&& k, M&& obj) {
        insert_pos pos;
        node_base* dup = find_unique_hint_pos(hint, k, pos);
        if (dup) {
            value_of(dup).second = std::forward<M>(obj);
            return iterator(dup);
        }
        node_base* z = create_node(piecewise_construct,
                                   forward_as_tuple(std::forward<K>(k)),
                                   forward_as_tuple(std::forward<M>(obj)));
        return insert_at(pos, z);
    }

    // ---- Multi emplace ----
    template<class... Args>
    iterator emplace_multi(Args&&... args) {
        node_base* z = create_node(std::forward<Args>(args)...);
        insert_pos pos;
        find_multi_pos(value_of(z), pos);
        return insert_at(pos, z);
    }
    template<class... Args>
    iterator emplace_hint_multi(const_iterator hint, Args&&... args) {
        node_base* z = create_node(std::forward<Args>(args)...);
        insert_pos pos;
        find_multi_hint_pos(hint, value_of(z), pos);
        return insert_at(pos, z);
    }

    // ---- Erase ----
    iterator erase(const_iterator pos) noexcept {
        node_base* z  = pos.node_;
        node_base* nx = detail::tree_increment(z);
        detail::tree_rebalance_for_erase(z, &header_);
        destroy_node(z);
        --size_;
        return iterator(nx);
    }
    iterator erase(const_iterator first, const_iterator last) noexcept {
        while (first != last) first = const_iterator(erase(first));
        return iterator(last.node_);
    }
    template<class K>
    size_type erase_unique(const K& k) {
        iterator it = find(k);
        if (it == end()) return 0;
        erase(const_iterator(it));
        return 1;
    }
    template<class K>
    size_type erase_multi(const K& k) {
        auto lo = lower_bound(k);
        auto hi = upper_bound(k);
        size_type n = 0;
        while (lo != hi) { lo = erase(const_iterator(lo)); ++n; }
        return n;
    }

    // ---- Lookup (thin, per-type descent) ----
    template<class K>
    iterator lower_bound(const K& k) {
        node_base* result = &header_;
        node_base* x = root_node();
        while (x) {
            if (!comp_(value_of(x), k)) { result = x; x = x->left; }
            else                        { x = x->right; }
        }
        return iterator(result);
    }
    template<class K>
    const_iterator lower_bound(const K& k) const {
        return const_cast<rb_tree*>(this)->lower_bound(k);
    }
    template<class K>
    iterator upper_bound(const K& k) {
        node_base* result = &header_;
        node_base* x = root_node();
        while (x) {
            if (comp_(k, value_of(x))) { result = x; x = x->left; }
            else                       { x = x->right; }
        }
        return iterator(result);
    }
    template<class K>
    const_iterator upper_bound(const K& k) const {
        return const_cast<rb_tree*>(this)->upper_bound(k);
    }
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
        auto lo = lower_bound(k), hi = upper_bound(k);
        size_type n = 0;
        for (auto it = lo; it != hi; ++it) ++n;
        return n;
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
};

} // namespace std
