// bits/vector_cores.h — non-template shared cores used by <vector> and
// <string> (and other contiguous-storage containers with the same shape).
//
// These primitives are keyed by:
//   * a `const detail::type_ops*` (element lifecycle leaves + size/align/flags)
//   * a `const detail::storage_ops*` (buffer state vocabulary) + void* st_ctx
//   * a void* el_ctx for the lifecycle leaves (the allocator instance, or
//     nullptr for stateless allocators).
//
// Storage_ops abstracts away whether the container stores three pointers
// (begin_/end_/cap_; Strategy A) or one pointer to a header-prefixed
// allocation (Strategy B); the cores read/write through the table.
//
// Conventions:
//   - Pointers and counts are in BYTES at this layer. The forwarder converts
//     T*/size_type/difference_type to byte pointers/counts at the boundary.
//   - Trivial-fast-path predicates are read off `tops->flags` (f_triv_X +
//     f_alloc_default_life).
//
// No exceptions, no RTTI. Allocation failure traps inside storage_ops::resize.
// constexpr is deliberately not used here (the cores are runtime-only).
#pragma once
#include <cstddef>
#include <bits/type_ops.h>
#include <bits/storage_ops.h>

namespace std {
namespace detail {

// ---------------------------------------------------------------------------
// Storage growth: ensure the buffer can hold at least `min_bytes` live
// bytes. Updates the storage state in place via storage_ops::resize.
// Used by reserve / resize-grow / shrink_to_fit / and the "ensure a spare
// slot" path before emplace_back's placement-new.
// ---------------------------------------------------------------------------
void grow(const type_ops* tops, const storage_ops* sops,
          void* st_ctx, void* el_ctx, size_t min_bytes);

// ---------------------------------------------------------------------------
// Destroy all elements in [begin, end) using tops->destroy (skipped when T is
// trivially destructible AND the allocator has default lifecycle).
// PRECONDITION trap: begin <= end.
// ---------------------------------------------------------------------------
void destroy_range(const type_ops* tops, void* el_ctx, void* begin, void* end);

// ---------------------------------------------------------------------------
// Default-construct n elements starting at `dst`. For trivially-default-
// constructible T + default lifecycle, value-init is a memset(0).
// ---------------------------------------------------------------------------
void construct_default_n(const type_ops* tops, void* el_ctx,
                         void* dst, size_t n);

// ---------------------------------------------------------------------------
// Copy-construct n elements at `dst`, each from `src` (one source object,
// broadcast). For trivially-copyable T + default lifecycle, this memcpys.
// ---------------------------------------------------------------------------
void construct_copy_one_n(const type_ops* tops, void* el_ctx,
                          void* dst, const void* src, size_t n);

// ---------------------------------------------------------------------------
// Rotate the constructed elements in [first, last) so that *middle becomes
// the new *first and *(middle - 1) becomes the new *(last - 1). Standard
// std::rotate semantics; type-erased.
//
// size_delta_bytes (signed): after rotating, adjust container size by this
// amount. Negative delta truncates: the trailing |delta| bytes are destroyed
// and dropped. Positive delta extends: the [middle, last) range — which the
// caller pre-constructed and rotated into [first, first+(last-middle)) — is
// folded into the live size. Delta = 0 leaves size unchanged (used when the
// caller already set size around an in-place rotation).
//
// Lets erase be a single core call (`rotate(p, p+1, end, -1*es)`) and insert
// be (`construct n at end; rotate(pos, end, end+n, +n*es)`) — no
// destruct_at_end / pop_back / set_size_elems_ in the forwarder.
// PRECONDITION traps: first <= middle <= last.
// ---------------------------------------------------------------------------
void rotate(const type_ops* tops, const storage_ops* sops,
            void* st_ctx, void* el_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes);

// ---------------------------------------------------------------------------
// Byte-level relocation primitive used by storage_ops::resize implementations
// to move live bytes from an old buffer to a freshly-allocated new one. For
// trivially-relocatable + default-lifecycle T this is a memcpy; otherwise it
// walks the elements via tops->move_construct + destroy. Used internally by
// storage_ops::resize; not normally called by container code directly.
// ---------------------------------------------------------------------------
void relocate_live(const type_ops* tops, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes);

} // namespace detail
} // namespace std
