// bits/vector_cores.h — non-template shared cores used by <vector> (and other
// containers / standalone algorithms with the same shape).
//
// These are general-purpose contiguous-storage primitives keyed by a
// `const detail::type_ops*` (element lifecycle leaves + size/align/flags) and,
// for the growth cores, a `realloc_op` callback bound to an opaque ctx.
//
// Naming convention: no `vec_` prefix — these are not vector-specific. A
// future <string> port consumes the same primitives, as do free-function
// algorithms (rotate, etc.).
//
// Conventions:
//   - Pointers and counts are in BYTES. The forwarder converts
//     T*/size_type/difference_type to byte pointers/counts at the boundary.
//   - Trivial-fast-path predicates are read off `ops->flags` (f_triv_X +
//     f_alloc_default_life). A null leaf pointer is NOT a "trivial" signal;
//     it means the leaf was elided by MASK or is ill-formed for T, and
//     calling it is a contract violation (and traps).
//
// No exceptions, no RTTI. Allocation failure traps inside realloc_op.
// constexpr is deliberately not used here (the cores are runtime-only); the
// methods that call them therefore lose constexpr too.
#pragma once
#include <cstddef>
#include <bits/type_ops.h>

namespace std {
namespace detail {

// State the growth cores mutate: byte pointers to begin/end/cap of the
// buffer. Packaged so a single forwarder call updates all three at once.
struct byte_buf {
    void* begin;
    void* end;
    void* cap;
};

// ---------------------------------------------------------------------------
// Storage growth (no gap): relocate-all into a buffer of at least
// `min_cap_bytes`. Used by reserve / resize-grow / shrink_to_fit / and the
// "ensure a spare slot" path before emplace_back's placement-new.
// Updates buf->begin / ->end / ->cap to the new buffer (byte pointers).
// ---------------------------------------------------------------------------
void grow(const type_ops* ops, realloc_op realloc, void* st_ctx,
          void* el_ctx, byte_buf* buf, size_t min_cap_bytes);

// ---------------------------------------------------------------------------
// Storage growth with a gap of `gap_bytes` at offset `gap_off_bytes`. Returns
// a byte pointer to the gap start in the new buffer; buf->end points PAST the
// moved tail (the gap slots are uninitialised — callers fill them, then this
// core does NOT bump end_ by the gap). Used by range-insert paths that grow.
// ---------------------------------------------------------------------------
void* grow_with_gap(const type_ops* ops, realloc_op realloc, void* st_ctx,
                    void* el_ctx, byte_buf* buf,
                    size_t gap_off_bytes, size_t gap_bytes,
                    size_t min_cap_bytes);

// ---------------------------------------------------------------------------
// In-place shift: open a `gap_bytes` hole at byte offset `off_bytes` inside
// the buffer that ends at `end_bytes`. PRECONDITION: T is trivially
// relocatable + default lifecycle (the caller dispatches; this core is the
// memmove specialisation).
// ---------------------------------------------------------------------------
void open_gap(void* base, size_t end_bytes, size_t off_bytes, size_t gap_bytes);

// ---------------------------------------------------------------------------
// Destroy all elements in [begin, end) using ops->destroy (skipped when T is
// trivially destructible AND the allocator has default lifecycle).
// ---------------------------------------------------------------------------
void destroy_range(const type_ops* ops, void* el_ctx, void* begin, void* end);

// ---------------------------------------------------------------------------
// Default-construct n elements starting at `dst`. For trivially-default-
// constructible T + default lifecycle, value-init is a memset(0).
// ---------------------------------------------------------------------------
void construct_default_n(const type_ops* ops, void* el_ctx,
                         void* dst, size_t n);

// ---------------------------------------------------------------------------
// Copy-construct n elements at `dst`, each from `src` (one source object,
// broadcast). For trivially-copyable T + default lifecycle, this memcpys.
// ---------------------------------------------------------------------------
void construct_copy_one_n(const type_ops* ops, void* el_ctx,
                          void* dst, const void* src, size_t n);

// ---------------------------------------------------------------------------
// Rotate the constructed elements in [first, last) so that *middle becomes
// the new *first and *(middle - 1) becomes the new *(last - 1). Standard
// std::rotate semantics; type-erased. For trivially-relocatable T + default
// lifecycle, uses scratch + two/three memcpys/memmoves; otherwise uses
// move_construct + destroy via leaves.
//
// Used by insert/emplace/erase: insert is `emplace_back; rotate(pos, last-1,
// last)`; erase is `rotate(pos, pos+1, end); pop_back`. Bulk variants take
// the analogous (n)-element block in middle position.
// ---------------------------------------------------------------------------
void rotate(const type_ops* ops, void* el_ctx,
            void* first, void* middle, void* last);

} // namespace detail
} // namespace std
