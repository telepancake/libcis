// bits/vector_cores.h — non-template cores for <vector>'s heavy methods.
//
// Declarations only; the bodies live once in src/support.cpp. Each core is
// keyed by:
//   - const detail::type_ops* ops   — element lifecycle leaves, sizeof/align
//   - detail::realloc_op           — raw byte-storage resize (for grow cores)
//   - void* el_ctx                  — element allocator instance (or null for
//                                     stateless / lifecycle-default allocators)
//   - void* st_ctx                  — storage ctx (the vector's `this`)
//
// Conventions:
//   - Pointers and counts are passed in BYTES. The forwarder converts
//     T*/size_type/difference_type to byte pointers/counts at the boundary.
//   - The realloc fast path (memcpy via realloc_op directly) is gated on
//     `ops->move_construct == nullptr` — the null sentinel for "trivially
//     relocatable AND default-lifecycle allocator." When the leaf is set, the
//     core constructs element-by-element via the leaf.
//
// No exceptions, no RTTI. Allocation failure traps inside realloc_op.
// constexpr is deliberately not used here (the cores are runtime-only); the
// <vector> methods that call them therefore lose constexpr too.
#pragma once
#include <cstddef>
#include <bits/type_ops.h>

namespace std {
namespace detail {

// State each core mutates: byte pointers to begin/end/cap of the buffer.
// Packaged so a single forwarder call updates all three at once.
struct vec_buf {
    void* begin;
    void* end;
    void* cap;
};

// ---------------------------------------------------------------------------
// Storage growth (no gap): relocate-all into a buffer of at least
// `min_cap_bytes`. Used by reserve / reallocate_grow / shrink_to_fit /
// emplace_back's slow path (when no element is being constructed in the gap).
// Updates buf->begin / ->end / ->cap to the new buffer (byte pointers).
// ---------------------------------------------------------------------------
void vec_grow(const type_ops* ops, realloc_op realloc, void* st_ctx,
              void* el_ctx, vec_buf* buf, size_t min_cap_bytes);

// ---------------------------------------------------------------------------
// Storage growth with a gap of `gap_bytes` at offset `gap_off_bytes`. Returns
// a byte pointer to the gap start in the new buffer; buf->end points PAST the
// moved tail (the gap slots are uninitialised — callers fill them, then this
// core does NOT bump end_ by the gap). Used by reallocate_and_insert_gap and
// by the emplace/insert grow paths.
// ---------------------------------------------------------------------------
void* vec_grow_with_gap(const type_ops* ops, realloc_op realloc, void* st_ctx,
                        void* el_ctx, vec_buf* buf,
                        size_t gap_off_bytes, size_t gap_bytes,
                        size_t min_cap_bytes);

// ---------------------------------------------------------------------------
// In-place shift: open a `gap_bytes` hole at byte offset `off_bytes` inside
// the buffer that ends at `end_bytes`. PRECONDITION: ops->move_construct ==
// nullptr (trivially relocatable + default lifecycle); the typed path
// otherwise calls move_range element-by-element.
// ---------------------------------------------------------------------------
void vec_open_gap(void* base, size_t end_bytes, size_t off_bytes, size_t gap_bytes);

// ---------------------------------------------------------------------------
// Destroy all elements in [begin_bytes, end_bytes) using ops->destroy (no-op
// when ops->destroy is null).
// ---------------------------------------------------------------------------
void vec_destroy_range(const type_ops* ops, void* el_ctx,
                       void* begin, void* end);

// ---------------------------------------------------------------------------
// Default-construct n elements starting at `dst`. ops->default_construct may
// be null (then it's a no-op for trivially-default-constructible T with a
// default-lifecycle allocator — already-zero memory left as-is).
// ---------------------------------------------------------------------------
void vec_construct_default_n(const type_ops* ops, void* el_ctx,
                             void* dst, size_t n);

// ---------------------------------------------------------------------------
// Copy-construct n elements at `dst`, each from `src` (one source object,
// broadcast). ops->copy_construct may be null for trivially-copyable + default
// lifecycle, in which case the core memcpy-broadcasts.
// ---------------------------------------------------------------------------
void vec_construct_copy_one_n(const type_ops* ops, void* el_ctx,
                              void* dst, const void* src, size_t n);

} // namespace detail
} // namespace std
