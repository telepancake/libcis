// bits/lean_array.h — single-instance structural kernels for lean vector<T>.
//
// These are NON-TEMPLATE inline functions: the linker keeps one copy per
// binary regardless of how many element types instantiate vector. They take a
// `void** slot` (the address of the container's one payload pointer) and a
// `const lean_ops*` type table (element size/align + relocate/copy/destroy
// thunks, null when the type is trivial). A null relocate means "trivially
// relocatable": the kernel grows through realloc and shifts through memmove
// with no per-element calls; otherwise it malloc's fresh storage and relocates
// through the thunks, then frees the old block.
//
// The per-T header keeps only the hot inline bits (the push_back capacity
// compare + placement new, and constructing individual cells from foreign
// iterators / constructor args); every bulk shuffle routes here.
#pragma once
#include <bits/lean.h>

namespace std {
namespace detail {

// Default visibility so the out-of-line kernel references below stay bindable
// to liblean.a / liblean.so even when a lean header is included inside a
// `#pragma GCC visibility push(hidden)` region (see bits/lean_string.h).
#pragma GCC visibility push(default)

// Elements of usable capacity for a live (non-null) payload.
inline size_t lean_arr_capacity(void* payload, const lean_ops* ops) noexcept {
  return payload ? lean_capacity_bytes(payload, ops->align) / ops->size : 0;
}

inline size_t lean_arr_size(void* payload) noexcept {
  return payload ? lean_used(payload) : 0;
}

// Allocate a fresh block with room for cap_elems elements; used starts at 0.
inline void* lean_arr_alloc_block(const lean_ops* ops, size_t cap_elems) noexcept {
  return lean_alloc(ops->align, cap_elems * ops->size);
}

// Grow-to-capacity: ensure the block holds at least min_elems elements,
// preserving the current elements and used count. No-op when already large
// enough. Handles a null slot (fresh allocation).
// (fat: defined out of line in lean/src/kernels.cpp.)
void lean_arr_reserve(void** slot, const lean_ops* ops, size_t min_elems) noexcept;

// Destroy the elements in [from, to); does NOT touch used (callers that shrink
// the logical size set used themselves — clear/pop/erase-tail/resize-shrink).
inline void lean_arr_destroy(void* payload, const lean_ops* ops,
                             size_t from, size_t to) noexcept {
  if (from < to && ops->destroy)
    ops->destroy(static_cast<char*>(payload) + from * ops->size, to - from);
}

// Erase [index, index+count): destroy those cells, shift the tail down into
// the hole, decrement used. In-place (no reallocation).
// (fat: defined out of line in lean/src/kernels.cpp.)
void lean_arr_erase(void** slot, const lean_ops* ops,
                    size_t index, size_t count) noexcept;

// Open a raw gap of `gap` cells at `index` by shifting [index, used) up.
// Precondition: capacity >= used + gap (caller reserved). Leaves the cells
// [index, index+gap) uninitialized for the caller to fill; bumps used by gap.
// (fat: defined out of line in lean/src/kernels.cpp.)
void lean_arr_open_gap(void* payload, const lean_ops* ops,
                       size_t index, size_t gap) noexcept;

// Finish a growing insert. `np` is a fresh block (from lean_arr_alloc_block)
// whose gap cells [index, index+gap) the caller has ALREADY filled (so a
// self-referential source is read while the old block is still alive). This
// relocates the old block's [0,index) to np[0,index) and [index,old_used) to
// np[index+gap, ...), frees the old block, sets used, and installs np.
// (fat: defined out of line in lean/src/kernels.cpp.)
void lean_arr_commit_gap(void** slot, const lean_ops* ops, void* np,
                         size_t index, size_t gap, size_t old_used) noexcept;

// assign-copy: make the container hold exactly n copies of the contiguous T
// range at `src`. Destroys the current contents, reuses the buffer when it is
// already large enough, else grows. `src` must not alias this container's
// storage (callers self-guard). copy==null with a non-trivial type never
// happens here — the copy path is only reached for copy-constructible T.
// (fat: defined out of line in lean/src/kernels.cpp.)
void lean_arr_assign_copy(void** slot, const lean_ops* ops,
                          const void* src, size_t n) noexcept;

// shrink-to-fit: drop spare capacity down to the current size (best effort;
// malloc rounding may still hand back a little slack). Frees to null when empty.
// (fat: defined out of line in lean/src/kernels.cpp.)
void lean_arr_shrink(void** slot, const lean_ops* ops) noexcept;

#pragma GCC visibility pop

} // namespace detail
} // namespace std
