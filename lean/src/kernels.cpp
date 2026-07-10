// lean/src/kernels.cpp — the one-copy-per-SYSTEM home for every fat, non-template
// structural/algorithm kernel of the lean profile.
//
// The lean bits headers (bits/lean_{string,array,tree,hash,deque,sort}.h and
// <list>) declare these kernels; their bodies live here so that a lean binary
// links ONE copy of each — buildable as liblean.a / liblean.so — instead of the
// per-binary COMDAT copy the old `inline` definitions produced.
//
// This translation unit contains NO templates and NO per-type code: every
// function operates on type-erased buffers plus a detail::lean_ops table (or a
// comparator/hash function pointer). It compiles with the exact project flags:
//   -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++
//   -Ilean/include -Iinclude
//
// The tiny hot accessors (lean_used/base/offset/alloc/realloc/free, the tree
// bit-0 color ops, hash_buckets, the sort byte-move/swap/elt helpers, list
// link/unlink, string term/fill/geo helpers) stay `inline` in the headers by
// design and are inlined into the kernels below.
#pragma GCC visibility push(default)
#include <bits/lean_string.h>
#include <bits/lean_array.h>
#include <bits/lean_tree.h>
#include <bits/lean_hash.h>
#include <bits/lean_deque.h>
#include <bits/lean_sort.h>
#include <list>
#pragma GCC visibility pop

namespace std {
namespace detail {

// The single shared empty-string representation (see bits/lean_string.h). One
// exported definition so its address is identical in every TU and across the
// liblean.so boundary — lean_str_is_static() is a pointer-identity test.
const lean_str_empty_rep lean_str_empty = {0, {}};

// ===========================================================================
// string — byte splice / reserve / shrink engine (bits/lean_string.h)
// ===========================================================================

void* lean_str_reserve(void* p, size_t elem, size_t need) noexcept {
  bool st = lean_str_is_static(p);
  if (!st && need <= lean_str_cap(p, elem))
    return p;
  size_t used = lean_used(p);
  if (st) {
    void* np = lean_alloc(elem, (need + 1) * elem);
    __builtin_memcpy(np, p, (used + 1) * elem);  // used == 0: copies terminator
    lean_used(np) = used;
    return np;
  }
  return lean_realloc(p, elem, (need + 1) * elem);
}

void* lean_str_shrink(void* p, size_t elem) noexcept {
  if (lean_str_is_static(p))
    return p;
  size_t used = lean_used(p);
  if (used == 0) {
    lean_free(p, elem);
    return lean_str_empty_payload();
  }
  if (used >= lean_str_cap(p, elem))
    return p;
  return lean_realloc(p, elem, (used + 1) * elem);
}

// Shared core for both splice variants. `is_fill == false`: `payload` is a byte
// source of n_add*elem bytes (alias-checked against the buffer). `is_fill ==
// true`: `payload` is a single element replicated n_add times (never aliases the
// buffer). The two public entry points are thin wrappers so callers are
// unchanged; the whole grow / in-place skeleton is written once.
static void* lean_str_splice_impl(void* p, size_t elem, size_t pos, size_t n_del,
                                  const void* payload, size_t n_add,
                                  bool is_fill) noexcept {
  size_t used = lean_used(p);
  size_t new_used = used - n_del + n_add;
  bool st = lean_str_is_static(p);
  if (st && new_used == 0)
    return p;  // still empty: stay on the static rep

  size_t cap = st ? 0 : lean_str_cap(p, elem);
  // A fill value is a single external element and never aliases the buffer.
  bool alias = !is_fill && !st && payload &&
               lean_str_in_range(payload, p, (cap + 1) * elem);

  if (st || new_used > cap || alias) {
    size_t geo = new_used > cap ? lean_str_geo(cap, new_used) : new_used;
    void* np = lean_alloc(elem, (geo + 1) * elem);
    char* d = static_cast<char*>(np);
    const char* o = static_cast<const char*>(p);
    if (pos)
      __builtin_memcpy(d, o, pos * elem);
    if (n_add) {                                    // payload in old block: live
      if (is_fill) lean_str_fill(d + pos * elem, elem, n_add, payload);
      else         __builtin_memcpy(d + pos * elem, payload, n_add * elem);
    }
    size_t suffix = used - pos - n_del;
    if (suffix)
      __builtin_memcpy(d + (pos + n_add) * elem,
                       o + (pos + n_del) * elem, suffix * elem);
    lean_str_write_term(np, new_used, elem);
    lean_used(np) = new_used;
    if (!st)
      lean_free(p, elem);
    return np;
  }

  // In place; payload does not alias the buffer.
  char* d = static_cast<char*>(p);
  size_t suffix = used - pos - n_del;
  if (suffix)
    __builtin_memmove(d + (pos + n_add) * elem,
                      d + (pos + n_del) * elem, suffix * elem);
  if (n_add) {
    if (is_fill) lean_str_fill(d + pos * elem, elem, n_add, payload);
    else         __builtin_memcpy(d + pos * elem, payload, n_add * elem);
  }
  lean_str_write_term(p, new_used, elem);
  lean_used(p) = new_used;
  return p;
}

void* lean_str_splice(void* p, size_t elem, size_t pos, size_t n_del,
                      const void* src, size_t n_add) noexcept {
  return lean_str_splice_impl(p, elem, pos, n_del, src, n_add, /*is_fill=*/false);
}

void* lean_str_splice_fill(void* p, size_t elem, size_t pos, size_t n_del,
                           size_t n_add, const void* one) noexcept {
  return lean_str_splice_impl(p, elem, pos, n_del, one, n_add, /*is_fill=*/true);
}

// ===========================================================================
// vector — grow / gap / erase / assign kernels (bits/lean_array.h)
// ===========================================================================

void lean_arr_reserve(void** slot, const lean_ops* ops, size_t min_elems) noexcept {
  void* p = *slot;
  if (lean_arr_capacity(p, ops) >= min_elems)
    return;
  size_t bytes = min_elems * ops->size;
  size_t used = p ? lean_used(p) : 0;
  if (!ops->relocate) {
    // Trivially relocatable: realloc keeps the bytes (and the header/used).
    void* np = p ? lean_realloc(p, ops->align, bytes)
                 : lean_alloc(ops->align, bytes);
    lean_used(np) = used;
    *slot = np;
  } else {
    void* np = lean_alloc(ops->align, bytes);
    if (p) {
      ops->relocate(np, p, used);   // move + end source lifetimes
      lean_free(p, ops->align);
    }
    lean_used(np) = used;
    *slot = np;
  }
}

void lean_arr_erase(void** slot, const lean_ops* ops,
                    size_t index, size_t count) noexcept {
  void* p = *slot;
  if (!p || count == 0)
    return;
  size_t used = lean_used(p);
  char* base = static_cast<char*>(p);
  lean_arr_destroy(p, ops, index, index + count);
  size_t tail = used - index - count;
  if (tail) {
    void* dst = base + index * ops->size;
    void* src = base + (index + count) * ops->size;
    if (ops->relocate)
      ops->relocate(dst, src, tail);     // dst < src: forward walk is safe
    else
      __builtin_memmove(dst, src, tail * ops->size);
  }
  lean_used(p) = used - count;
}

void lean_arr_open_gap(void* payload, const lean_ops* ops,
                       size_t index, size_t gap) noexcept {
  size_t used = lean_used(payload);
  char* base = static_cast<char*>(payload);
  size_t tail = used - index;
  if (tail) {
    void* dst = base + (index + gap) * ops->size;
    void* src = base + index * ops->size;
    if (ops->relocate_backward)
      ops->relocate_backward(dst, src, tail);  // dst > src, overlap: walk end
    else
      __builtin_memmove(dst, src, tail * ops->size);
  }
  lean_used(payload) = used + gap;
}

void lean_arr_commit_gap(void** slot, const lean_ops* ops, void* np,
                         size_t index, size_t gap, size_t old_used) noexcept {
  void* p = *slot;
  char* nb = static_cast<char*>(np);
  if (p) {
    char* ob = static_cast<char*>(p);
    size_t tail = old_used - index;
    if (ops->relocate) {
      if (index)
        ops->relocate(nb, ob, index);
      if (tail)
        ops->relocate(nb + (index + gap) * ops->size,
                      ob + index * ops->size, tail);
    } else {
      if (index)
        __builtin_memcpy(nb, ob, index * ops->size);
      if (tail)
        __builtin_memcpy(nb + (index + gap) * ops->size,
                         ob + index * ops->size, tail * ops->size);
    }
    lean_free(p, ops->align);
  }
  lean_used(np) = old_used + gap;
  *slot = np;
}

void lean_arr_assign_copy(void** slot, const lean_ops* ops,
                          const void* src, size_t n) noexcept {
  void* p = *slot;
  if (p) {
    lean_arr_destroy(p, ops, 0, lean_used(p));
    lean_used(p) = 0;
  }
  if (n == 0)
    return;
  lean_arr_reserve(slot, ops, n);        // used is 0, so nothing to relocate
  p = *slot;
  if (ops->copy)
    ops->copy(p, src, n);
  else
    __builtin_memcpy(p, src, n * ops->size);
  lean_used(p) = n;
}

void lean_arr_shrink(void** slot, const lean_ops* ops) noexcept {
  void* p = *slot;
  if (!p)
    return;
  size_t used = lean_used(p);
  if (used == 0) {
    lean_free(p, ops->align);
    *slot = nullptr;
    return;
  }
  if (lean_arr_capacity(p, ops) == used)
    return;
  size_t bytes = used * ops->size;
  if (!ops->relocate) {
    void* np = lean_realloc(p, ops->align, bytes);
    lean_used(np) = used;
    *slot = np;
  } else {
    void* np = lean_alloc(ops->align, bytes);
    ops->relocate(np, p, used);
    lean_free(p, ops->align);
    lean_used(np) = used;
    *slot = np;
  }
}

// ===========================================================================
// rb-tree — rotate / rebalance / erase-fixup / iterate / min-max / destroy
// (bits/lean_tree.h)
// ===========================================================================

tree_node_base* tree_min(tree_node_base* x) noexcept {
  while (x->left) x = x->left;
  return x;
}
tree_node_base* tree_max(tree_node_base* x) noexcept {
  while (x->right) x = x->right;
  return x;
}

tree_node_base* tree_increment(tree_node_base* x) noexcept {
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
tree_node_base* tree_decrement(tree_node_base* x) noexcept {
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

// Internal to this TU: the two rotations are only called by the rebalancers.
static void tree_rotate_left(tree_node_base* x, tree_node_base* header) noexcept {
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
static void tree_rotate_right(tree_node_base* x, tree_node_base* header) noexcept {
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

void tree_insert_and_rebalance(bool insert_left, tree_node_base* x,
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

tree_node_base* tree_rebalance_for_erase(tree_node_base* z,
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

void tree_destroy(tree_node_base* n,
                  void (*destroy_node)(tree_node_base*)) noexcept {
  while (n) {
    tree_destroy(n->left, destroy_node);
    tree_node_base* r = n->right;
    destroy_node(n);
    n = r;
  }
}

// ===========================================================================
// hash — rehash / relink / unlink / destroy kernels (bits/lean_hash.h)
// ===========================================================================

void hash_rehash_into(hash_control* ctl) noexcept {
  size_t nbc = ctl->bucket_count;
  size_t mask = nbc - 1;
  hnode_base** buckets = hash_buckets(ctl);
  hnode_base* pp = &ctl->first;
  hnode_base* cp = pp->next;
  if (cp == nullptr)
    return;
  size_t chash = cp->hash & mask;
  buckets[chash] = pp;
  size_t phash = chash;
  for (pp = cp, cp = cp->next; cp != nullptr; cp = pp->next) {
    chash = cp->hash & mask;
    if (chash == phash) {
      pp = cp;
    } else if (buckets[chash] == nullptr) {
      buckets[chash] = pp;
      pp = cp;
      phash = chash;
    } else {
      // A run for this bucket already exists earlier in the list: splice
      // cp in right behind that run's head so the bucket stays contiguous.
      hnode_base* np = cp;
      pp->next = np->next;
      np->next = buckets[chash]->next;
      buckets[chash]->next = cp;
    }
  }
}

hash_control* hash_set_bucket_count(hash_control* ctl, size_t nbc) noexcept {
  size_t bytes = sizeof(hash_control) + nbc * sizeof(hnode_base*);
  if (ctl == nullptr) {
    ctl = static_cast<hash_control*>(::malloc(bytes));
    if (ctl == nullptr)
      __builtin_trap();
    ctl->size = 0;
    ctl->max_load_factor = 1.0f;
    ctl->first.next = nullptr;
    ctl->first.hash = 0;
  } else {
    ctl = static_cast<hash_control*>(::realloc(ctl, bytes));
    if (ctl == nullptr)
      __builtin_trap();
  }
  ctl->bucket_count = nbc;
  hnode_base** buckets = hash_buckets(ctl);
  for (size_t i = 0; i < nbc; ++i)
    buckets[i] = nullptr;
  hash_rehash_into(ctl);
  return ctl;
}

void hash_link_unique(hash_control* ctl, hnode_base* nd) noexcept {
  size_t mask = ctl->bucket_count - 1;
  size_t chash = nd->hash & mask;
  hnode_base** buckets = hash_buckets(ctl);
  hnode_base* pn = buckets[chash];
  if (pn == nullptr) {
    // New bucket: splice at the head of the whole list.
    pn = &ctl->first;
    nd->next = pn->next;
    pn->next = nd;
    buckets[chash] = pn;
    if (nd->next != nullptr)
      // The old head node's bucket now has `nd` as its predecessor.
      buckets[nd->next->hash & mask] = nd;
  } else {
    nd->next = pn->next;
    pn->next = nd;
  }
  ++ctl->size;
}

void hash_unlink(hash_control* ctl, hnode_base* cn) noexcept {
  size_t mask = ctl->bucket_count - 1;
  size_t chash = cn->hash & mask;
  hnode_base** buckets = hash_buckets(ctl);
  hnode_base* pn = buckets[chash];
  while (pn->next != cn)
    pn = pn->next;
  // If cn was the first in its bucket run, and it is also the last, the bucket
  // becomes empty.
  if (pn == &ctl->first || (pn->hash & mask) != chash) {
    if (cn->next == nullptr || (cn->next->hash & mask) != chash)
      buckets[chash] = nullptr;
  }
  // If a different bucket run follows cn, its predecessor becomes pn.
  if (cn->next != nullptr) {
    size_t nhash = cn->next->hash & mask;
    if (nhash != chash)
      buckets[nhash] = pn;
  }
  pn->next = cn->next;
  cn->next = nullptr;
  --ctl->size;
}

void hash_destroy_all(hash_control* ctl, void (*destroy)(hnode_base*)) noexcept {
  hnode_base* np = ctl->first.next;
  while (np != nullptr) {
    hnode_base* nx = np->next;
    destroy(np);
    np = nx;
  }
  ctl->first.next = nullptr;
  hnode_base** buckets = hash_buckets(ctl);
  for (size_t i = 0; i < ctl->bucket_count; ++i)
    buckets[i] = nullptr;
  ctl->size = 0;
}

// ===========================================================================
// deque — block-table growth / recenter / destroy (bits/lean_deque.h)
// ===========================================================================

lean_deque_reserve
lean_deque_reserve_front(void** table, size_t lo, size_t nb) noexcept {
  size_t cap = lean_deque_table_cap(table);
  if (lo >= 1)
    return {table, 0};
  size_t back_spare = cap - (lo + nb);           // free slots above the range
  if (back_spare >= 2) {
    size_t new_lo = (cap - nb) / 2;              // >= 1 (cap - nb == back_spare)
    ::memmove(table + new_lo, table + lo, nb * sizeof(void*));
    return {table, static_cast<long>(new_lo) - static_cast<long>(lo)};
  }
  size_t new_cap = cap < 8 ? 8 : cap * 2;
  if (new_cap < nb + 2)
    new_cap = nb + 2;
  void** nt = table
      ? static_cast<void**>(lean_realloc(table, alignof(void*), new_cap * sizeof(void*)))
      : lean_deque_table_alloc(new_cap);
  new_cap = lean_deque_table_cap(nt);
  size_t new_lo = (new_cap - nb) / 2;
  if (new_lo < 1)
    new_lo = 1;
  ::memmove(nt + new_lo, nt + lo, nb * sizeof(void*));
  return {nt, static_cast<long>(new_lo) - static_cast<long>(lo)};
}

lean_deque_reserve
lean_deque_reserve_back(void** table, size_t lo, size_t nb) noexcept {
  size_t cap = lean_deque_table_cap(table);
  if (cap && lo + nb < cap)
    return {table, 0};
  size_t front_spare = lo;                       // free slots below the range
  if (front_spare >= 2) {
    size_t new_lo = (cap - nb) / 2;              // < lo, so a slot opens above
    ::memmove(table + new_lo, table + lo, nb * sizeof(void*));
    return {table, static_cast<long>(new_lo) - static_cast<long>(lo)};
  }
  size_t new_cap = cap < 8 ? 8 : cap * 2;
  if (new_cap < nb + 2)
    new_cap = nb + 2;
  void** nt = table
      ? static_cast<void**>(lean_realloc(table, alignof(void*), new_cap * sizeof(void*)))
      : lean_deque_table_alloc(new_cap);
  new_cap = lean_deque_table_cap(nt);
  size_t new_lo = (new_cap - nb) / 2;
  ::memmove(nt + new_lo, nt + lo, nb * sizeof(void*));
  return {nt, static_cast<long>(new_lo) - static_cast<long>(lo)};
}

void lean_deque_destroy_all(void** table, size_t off, size_t count,
                            const lean_ops* ops) noexcept {
  if (!ops->destroy || count == 0)
    return;
  size_t be = lean_deque_block_elems(ops->size);
  size_t abs = off, remaining = count;
  while (remaining) {
    size_t slot = abs / be, in = abs % be;
    size_t here = be - in;
    if (here > remaining)
      here = remaining;
    ops->destroy(static_cast<char*>(table[slot]) + in * ops->size, here);
    abs += here;
    remaining -= here;
  }
}

// ===========================================================================
// list — reverse / merge-sort frame (<list>)
// ===========================================================================

// Internal to this TU: advance a raw node link n hops (used only by list_sort).
static list_node_base* list_advance(list_node_base* p, size_t n) noexcept {
  for (; n != 0; --n) p = p->next;
  return p;
}

void list_reverse(list_node_base* s) noexcept {
  list_node_base* cur = s;
  do {
    list_node_base* nxt = cur->next;
    cur->next = cur->prev;
    cur->prev = nxt;
    cur = nxt;
  } while (cur != s);
}

list_node_base* list_sort(list_node_base* f1, list_node_base* e2, size_t n,
                          list_less_fn comp, void* ctx) {
  switch (n) {
  case 0:
  case 1:
    return f1;
  case 2: {
    list_node_base* lm1 = e2->prev;
    if (comp(lm1, f1, ctx)) {
      list_unlink(lm1, lm1);
      list_link(f1, lm1, lm1);
      return lm1;
    }
    return f1;
  }
  }
  size_t n2 = n / 2;
  list_node_base* e1 = list_advance(f1, n2);
  list_node_base* r  = f1 = list_sort(f1, e1, n2, comp, ctx);
  list_node_base* f2 = e1 = list_sort(e1, e2, n - n2, comp, ctx);
  if (comp(f2, f1, ctx)) {
    list_node_base* m2 = f2->next;
    for (; m2 != e2 && comp(m2, f1, ctx); m2 = m2->next) ;
    list_node_base* ff = f2;
    list_node_base* ll = m2->prev;
    r  = f2;
    e1 = f2 = m2;
    list_unlink(ff, ll);
    m2 = f1->next;
    list_link(f1, ff, ll);
    f1 = m2;
  } else {
    f1 = f1->next;
  }
  while (f1 != e1 && f2 != e2) {
    if (comp(f2, f1, ctx)) {
      list_node_base* m2 = f2->next;
      for (; m2 != e2 && comp(m2, f1, ctx); m2 = m2->next) ;
      list_node_base* ff = f2;
      list_node_base* ll = m2->prev;
      if (e1 == f2) e1 = m2;
      f2 = m2;
      list_unlink(ff, ll);
      m2 = f1->next;
      list_link(f1, ff, ll);
      f1 = m2;
    } else {
      f1 = f1->next;
    }
  }
  return r;
}

// ===========================================================================
// algorithm — introsort / heap / merge-sort kernels (bits/lean_sort.h)
//
// The internal leaves (sift/insertion/partition/heap-range/merge-runs) are file
// -local; only the entry points the <algorithm> overlay names get external
// linkage. The tiny byte-move/swap/elt/log2 helpers stay inline in the header.
// ===========================================================================

static void lean_sift_down(char* base, size_t elem, size_t n, size_t start,
                           lean_less_fn less, void* ctx, char* tmp) {
  lean_move_bytes(tmp, lean_elt(base, start, elem), elem);
  size_t root = start;
  for (;;) {
    size_t child = 2 * root + 1;
    if (child >= n) break;
    if (child + 1 < n &&
        less(lean_elt(base, child, elem), lean_elt(base, child + 1, elem), ctx))
      ++child;
    if (!less(tmp, lean_elt(base, child, elem), ctx))  // held >= child: settled
      break;
    lean_move_bytes(lean_elt(base, root, elem), lean_elt(base, child, elem), elem);
    root = child;
  }
  lean_move_bytes(lean_elt(base, root, elem), tmp, elem);
}

static void lean_sift_up(char* base, size_t elem, size_t n,
                         lean_less_fn less, void* ctx, char* tmp) {
  if (n < 2) return;
  size_t child = n - 1;
  lean_move_bytes(tmp, lean_elt(base, child, elem), elem);
  while (child > 0) {
    size_t parent = (child - 1) / 2;
    if (!less(lean_elt(base, parent, elem), tmp, ctx))  // parent >= held: settled
      break;
    lean_move_bytes(lean_elt(base, child, elem), lean_elt(base, parent, elem), elem);
    child = parent;
  }
  lean_move_bytes(lean_elt(base, child, elem), tmp, elem);
}

static void lean_sort_heap_range(char* base, size_t n, size_t elem,
                                 lean_less_fn less, void* ctx, char* tmp) {
  for (size_t i = n; i > 1; --i) {
    lean_swap_bytes(base, lean_elt(base, i - 1, elem), elem, tmp);
    lean_sift_down(base, elem, i - 1, 0, less, ctx, tmp);
  }
}

static void lean_heap_sort_range(char* base, size_t n, size_t elem,
                                 lean_less_fn less, void* ctx, char* tmp) {
  for (size_t i = n / 2; i-- > 0;)
    lean_sift_down(base, elem, n, i, less, ctx, tmp);
  lean_sort_heap_range(base, n, elem, less, ctx, tmp);
}

static void lean_insertion_sort(char* base, size_t n, size_t elem,
                                lean_less_fn less, void* ctx, char* tmp) {
  for (size_t i = 1; i < n; ++i) {
    if (less(lean_elt(base, i, elem), lean_elt(base, i - 1, elem), ctx)) {
      lean_move_bytes(tmp, lean_elt(base, i, elem), elem);
      size_t j = i;
      do {
        lean_move_bytes(lean_elt(base, j, elem), lean_elt(base, j - 1, elem), elem);
        --j;
      } while (j > 0 && less(tmp, lean_elt(base, j - 1, elem), ctx));
      lean_move_bytes(lean_elt(base, j, elem), tmp, elem);
    }
  }
}

static size_t lean_partition(char* base, size_t n, size_t elem,
                             lean_less_fn less, void* ctx, char* tmp, char* pivot) {
  char* a   = base;
  char* mid = lean_elt(base, n / 2, elem);
  char* last = lean_elt(base, n - 1, elem);
  if (less(mid, a, ctx)) lean_swap_bytes(a, mid, elem, tmp);
  if (less(last, mid, ctx)) {
    lean_swap_bytes(mid, last, elem, tmp);
    if (less(mid, a, ctx)) lean_swap_bytes(a, mid, elem, tmp);
  }
  // a <= mid <= last by value; median is at mid. Move it to index 0.
  lean_swap_bytes(a, mid, elem, tmp);
  lean_move_bytes(pivot, a, elem);

  size_t i = 0, j = n;
  for (;;) {
    do { ++i; } while (i < n && less(lean_elt(base, i, elem), pivot, ctx));
    do { --j; } while (less(pivot, lean_elt(base, j, elem), ctx));
    if (i >= j) break;
    lean_swap_bytes(lean_elt(base, i, elem), lean_elt(base, j, elem), elem, tmp);
  }
  lean_swap_bytes(base, lean_elt(base, j, elem), elem, tmp);
  return j;
}

static void lean_introsort(char* base, size_t n, size_t elem,
                           lean_less_fn less, void* ctx, size_t depth,
                           char* tmp, char* pivot) {
  const size_t cutoff = 16;
  while (n > cutoff) {
    if (depth == 0) {
      lean_heap_sort_range(base, n, elem, less, ctx, tmp);
      return;
    }
    --depth;
    size_t j = lean_partition(base, n, elem, less, ctx, tmp, pivot);
    size_t leftn  = j;
    size_t rightn = n - j - 1;
    // Recurse into the smaller side, loop on the larger (bounded stack depth).
    if (leftn < rightn) {
      lean_introsort(base, leftn, elem, less, ctx, depth, tmp, pivot);
      base = lean_elt(base, j + 1, elem);
      n = rightn;
    } else {
      lean_introsort(lean_elt(base, j + 1, elem), rightn, elem, less, ctx, depth, tmp, pivot);
      n = leftn;
    }
  }
  lean_insertion_sort(base, n, elem, less, ctx, tmp);
}

static void lean_merge_runs(const char* src, char* dst,
                            size_t left, size_t mid, size_t right, size_t elem,
                            lean_less_fn less, void* ctx) {
  size_t i = left, j = mid, k = left;
  while (i < mid && j < right) {
    if (less(src + j * elem, src + i * elem, ctx)) {  // right < left: take right
      lean_move_bytes(dst + k * elem, src + j * elem, elem);
      ++j;
    } else {                                           // equal: take left (stable)
      lean_move_bytes(dst + k * elem, src + i * elem, elem);
      ++i;
    }
    ++k;
  }
  while (i < mid)   { lean_move_bytes(dst + k * elem, src + i * elem, elem); ++i; ++k; }
  while (j < right) { lean_move_bytes(dst + k * elem, src + j * elem, elem); ++j; ++k; }
}

// ---- public entry points (named by the <algorithm> overlay) --------------

void lean_make_heap(char* base, size_t n, size_t elem,
                    lean_less_fn less, void* ctx) {
  if (n < 2) return;
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  for (size_t i = n / 2; i-- > 0;)
    lean_sift_down(base, elem, n, i, less, ctx, tmp);
}

void lean_push_heap(char* base, size_t n, size_t elem,
                    lean_less_fn less, void* ctx) {
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  lean_sift_up(base, elem, n, less, ctx, tmp);
}

void lean_pop_heap(char* base, size_t n, size_t elem,
                   lean_less_fn less, void* ctx) {
  if (n < 2) return;
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  lean_swap_bytes(base, lean_elt(base, n - 1, elem), elem, tmp);
  lean_sift_down(base, elem, n - 1, 0, less, ctx, tmp);
}

void lean_sort_heap(char* base, size_t n, size_t elem,
                    lean_less_fn less, void* ctx) {
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  lean_sort_heap_range(base, n, elem, less, ctx, tmp);
}

void lean_sort(char* base, size_t n, size_t elem,
               lean_less_fn less, void* ctx) {
  if (n < 2) return;
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  alignas(max_align_t) char pivot[lean_sort_max_elem];
  lean_introsort(base, n, elem, less, ctx, 2 * lean_log2(n), tmp, pivot);
}

void lean_partial_sort(char* base, size_t m, size_t n, size_t elem,
                       lean_less_fn less, void* ctx) {
  if (m == 0) return;
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  for (size_t i = m / 2; i-- > 0;)
    lean_sift_down(base, elem, m, i, less, ctx, tmp);
  for (size_t i = m; i < n; ++i) {
    if (less(lean_elt(base, i, elem), base, ctx)) {  // tail element < heap max
      lean_swap_bytes(base, lean_elt(base, i, elem), elem, tmp);
      lean_sift_down(base, elem, m, 0, less, ctx, tmp);
    }
  }
  lean_sort_heap_range(base, m, elem, less, ctx, tmp);
}

void lean_nth_element(char* base, size_t nth, size_t n, size_t elem,
                      lean_less_fn less, void* ctx) {
  if (n < 2 || nth >= n) return;
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  alignas(max_align_t) char pivot[lean_sort_max_elem];
  size_t depth = 2 * lean_log2(n);
  const size_t cutoff = 16;
  while (n > cutoff) {
    if (depth == 0) {
      lean_heap_sort_range(base, n, elem, less, ctx, tmp);
      return;
    }
    --depth;
    size_t j = lean_partition(base, n, elem, less, ctx, tmp, pivot);
    if (nth == j) return;
    if (nth < j) {
      n = j;
    } else {
      base = lean_elt(base, j + 1, elem);
      nth -= j + 1;
      n -= j + 1;
    }
  }
  lean_insertion_sort(base, n, elem, less, ctx, tmp);
}

void lean_stable_sort(char* base, size_t n, size_t elem,
                      lean_less_fn less, void* ctx) {
  if (n < 2) return;
  alignas(max_align_t) char tmp[lean_sort_max_elem];
  const size_t run = 16;
  for (size_t i = 0; i < n; i += run) {
    size_t len = n - i < run ? n - i : run;
    lean_insertion_sort(base + i * elem, len, elem, less, ctx, tmp);
  }
  if (n <= run) return;
  char* buf = static_cast<char*>(::malloc(n * elem));
  if (!buf) __builtin_trap();
  char* src = base;
  char* dst = buf;
  for (size_t width = run; width < n; width *= 2) {
    for (size_t i = 0; i < n; i += 2 * width) {
      size_t mid   = i + width   < n ? i + width   : n;
      size_t right = i + 2 * width < n ? i + 2 * width : n;
      lean_merge_runs(src, dst, i, mid, right, elem, less, ctx);
    }
    char* t = src; src = dst; dst = t;
  }
  if (src != base)
    lean_move_bytes(base, src, n * elem);
  ::free(buf);
}

void lean_inplace_merge(char* base, size_t len1, size_t len2, size_t elem,
                        lean_less_fn less, void* ctx) {
  if (len1 == 0 || len2 == 0) return;
  size_t total = len1 + len2;
  if (len1 <= len2) {
    char* buf = static_cast<char*>(::malloc(len1 * elem));
    if (!buf) __builtin_trap();
    lean_move_bytes(buf, base, len1 * elem);  // stash the left run
    size_t i = 0, j = len1, k = 0;
    while (i < len1 && j < total) {
      if (less(base + j * elem, buf + i * elem, ctx)) {  // right < left
        lean_move_bytes(base + k * elem, base + j * elem, elem); ++j;
      } else {                                            // equal: take left (stable)
        lean_move_bytes(base + k * elem, buf + i * elem, elem); ++i;
      }
      ++k;
    }
    while (i < len1) { lean_move_bytes(base + k * elem, buf + i * elem, elem); ++i; ++k; }
    // remaining right elements are already in place
    ::free(buf);
  } else {
    char* buf = static_cast<char*>(::malloc(len2 * elem));
    if (!buf) __builtin_trap();
    lean_move_bytes(buf, base + len1 * elem, len2 * elem);  // stash the right run
    // Merge from the top down so the in-place left run is never clobbered
    // before it is read. i/j/k are counts remaining; k == i + j is invariant.
    size_t i = len2, j = len1, k = total;
    while (i > 0 && j > 0) {
      if (less(buf + (i - 1) * elem, base + (j - 1) * elem, ctx)) {
        // left tail is the larger: it goes last
        --k; lean_move_bytes(base + k * elem, base + (j - 1) * elem, elem); --j;
      } else {
        // equal or right larger: take right (stable — right after left)
        --k; lean_move_bytes(base + k * elem, buf + (i - 1) * elem, elem); --i;
      }
    }
    while (i > 0) { --k; lean_move_bytes(base + k * elem, buf + (i - 1) * elem, elem); --i; }
    // remaining left elements are already in place
    ::free(buf);
  }
}

} // namespace detail
} // namespace std
