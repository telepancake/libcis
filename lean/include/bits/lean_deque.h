// lean/include/bits/lean_deque.h — type-erased structural kernels for the
// lean deque. These are the non-template, single-instance-per-binary functions
// the deque contract asks for: block allocate/free, block-table growth and
// recentering, and bulk destroy. They take a const detail::lean_ops* so one
// linked copy serves every element type; the per-T header keeps only the thin
// leaves (iterator arithmetic, element access, construct/destroy of one slot).
//
// Representation (see <deque>):
//   - A *block* is a raw ::malloc'd array of `block_elems` elements. malloc's
//     alignment covers any T with alignof(T) <= alignof(max_align_t); the
//     container static_asserts that bound, so blocks need no lean header.
//   - The *table* is a lean block (bits/lean.h) of void* block base pointers.
//     Its capacity in slots is recovered from malloc_usable_size for free.
//   - Growing the deque never moves an element: only block *pointers* move
//     inside the table (a memmove of void*), so element addresses — and hence
//     references — stay valid at both ends, exactly as the standard requires.
#pragma once
#include <bits/lean.h>
#include <cstring>   // memmove

namespace std {
namespace detail {

// Fixed BYTE budget per block. A block holds max(1, budget/sizeof(T)) elements.
inline constexpr size_t lean_deque_block_bytes = 512;

// The element count per block for a given element size. MUST agree with the
// container's constexpr `be` (same clamped-integer-divide formula).
inline size_t lean_deque_block_elems(size_t elem_size) noexcept {
  size_t be = lean_deque_block_bytes / (elem_size ? elem_size : 1);
  return be ? be : 1;
}

// One block: raw storage for block_elems(ops->size) elements.
inline void* lean_deque_alloc_block(const lean_ops* ops) noexcept {
  void* p = ::malloc(lean_deque_block_elems(ops->size) * ops->size);
  if (!p)
    __builtin_trap();
  return p;
}
inline void lean_deque_free_block(void* block) noexcept { ::free(block); }

// The table is a lean block of void*.
inline void** lean_deque_table_alloc(size_t slots) noexcept {
  return static_cast<void**>(lean_alloc(alignof(void*), slots * sizeof(void*)));
}
inline size_t lean_deque_table_cap(void** table) noexcept {
  return table ? lean_capacity_bytes(table, alignof(void*)) / sizeof(void*) : 0;
}
inline void lean_deque_table_free(void** table) noexcept {
  if (table)
    lean_free(table, alignof(void*));
}

// Result of reserving a free table slot: a possibly-reallocated table and the
// signed slot shift applied to the live block range. The caller adjusts its
// front offset by shift * block_elems (block pointers moved, elements did not).
struct lean_deque_reserve {
  void** table;
  long   shift;
};

// Ensure a free table slot exists BELOW the live range [lo, lo+nb) (i.e. the
// resulting first-live-slot is >= 1). Recenter in place when the far side has
// spare, otherwise grow geometrically and centre.
inline lean_deque_reserve
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

// Ensure a free table slot exists ABOVE the live range [lo, lo+nb) (i.e.
// lo + nb < cap afterwards). Symmetric to reserve_front.
inline lean_deque_reserve
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

// Destroy `count` elements starting at absolute index `off`, walking blocks.
// Null ops->destroy (trivial type) => nothing to do.
inline void lean_deque_destroy_all(void** table, size_t off, size_t count,
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

// Free every live block in table slots [first_slot, last_slot].
inline void lean_deque_free_blocks(void** table, size_t first_slot,
                                   size_t last_slot) noexcept {
  for (size_t s = first_slot; s <= last_slot; ++s)
    lean_deque_free_block(table[s]);
}

} // namespace detail
} // namespace std
