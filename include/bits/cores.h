// bits/cores.h — non-template shared cores used by <vector> and
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
#include <bits/segment_map_ops.h>

namespace std {
namespace detail {

// ---------------------------------------------------------------------------
// All public cores follow the same context shape:
//
//   (type_ops*, storage_ops*, st_ctx, ...op-specific args...)
//
// The element ctx (`el_ctx`, the allocator instance pointer) is fetched
// LAZILY by the core via `sops->alloc_ctx(st_ctx)` only when the type_ops
// flags say the allocator has per-instance state (`f_alloc_ctx` set). For
// stateless allocators (std::allocator, min_allocator, ...) and trivial
// fast-path operations, the fetch never happens — no el_ctx threading.
// ---------------------------------------------------------------------------

// Storage growth: ensure the buffer can hold at least `min_bytes` live bytes.
// Updates the storage state in place via storage_ops::resize.
void grow(const type_ops* tops, const storage_ops* sops,
          void* st_ctx, size_t min_bytes);

// Destroy all elements in [begin, end) using tops->destroy (skipped when T is
// trivially destructible AND the allocator has default lifecycle).
// PRECONDITION trap: begin <= end.
void destroy_range(const type_ops* tops, const storage_ops* sops,
                   void* st_ctx, void* begin, void* end);

// Copy-construct n elements at `dst`, each from `src` (one source object,
// broadcast). For trivially-copyable T + default lifecycle, this memcpys.
void construct_copy_one_n(const type_ops* tops, const storage_ops* sops,
                          void* st_ctx, void* dst, const void* src, size_t n);

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
void rotate(const type_ops* tops, const storage_ops* sops, void* st_ctx,
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

// ---------------------------------------------------------------------------
// Segmented (deque) cores.
//
// Where the contiguous cores above operate on a single byte range, the
// segmented cores walk a segment chain through `segment_map_ops`. Iterators
// at this layer are (segment_base, in-segment byte pointer) pairs carried as
// two void*s; the container converts at the boundary.
//
// All three share the same trivial fast paths as their contiguous siblings:
//   destroy   -> skip when (f_triv_destroy & f_alloc_default_life)
//   copy/move -> memcpy a segment-aligned run when (f_triv_X &
//                f_alloc_default_life), else walk elements via the leaves.
//
// PRECONDITION traps: (first.seg, first.ptr) <= (last.seg, last.ptr) along
// the live chain, in the same total ordering deque_iterator uses.
// ---------------------------------------------------------------------------

// Destroy [first, last) walking the chain segment by segment. Trivial-X +
// default-life T skip the destroy entirely.
void segmented_destroy(const type_ops* tops, const segment_map_ops* mops,
                       void* st_ctx,
                       void* first_seg, void* first_ptr,
                       void* last_seg,  void* last_ptr);

// Move-construct [src_first, src_last) into the segments starting at
// dst_first, walking both source and destination segment chains. The two
// segment_map_ops can name DIFFERENT deques (e.g. cross-deque assignment) or
// the same one (the common case). When `is_copy` is true, copy_construct is
// used; otherwise move_construct. The destination range is expected to be
// already-laid-out raw segments (no live elements there yet) — the cores'
// responsibility is the construct, not the segment growth.
void segmented_copy(const type_ops* tops,
                    const segment_map_ops* src_mops,
                    const segment_map_ops* dst_mops,
                    void* src_ctx, void* dst_ctx,
                    void* src_first_seg, void* src_first_ptr,
                    void* src_last_seg,  void* src_last_ptr,
                    void* dst_first_seg, void* dst_first_ptr,
                    bool is_copy);

// Segment-aware rotate of [first, last) so that *middle becomes the new
// *first. Algorithm: gather the smaller block into scratch (stack ≤ 256 B,
// ::malloc above), shift the larger block, restore. Trivial-reloc + default-
// life T degenerates to per-segment memcpy runs.
//
// size_delta_bytes (signed): after rotating, adjust the deque's logical size
// by this amount. Negative truncates from the back; positive extends. The
// caller is responsible for any segment push_back_segment / pop_back_segment
// needed before the call; the core walks within the already-existing chain.
void segmented_rotate(const type_ops* tops, const segment_map_ops* mops,
                      void* st_ctx,
                      void* first_seg,  void* first_ptr,
                      void* middle_seg, void* middle_ptr,
                      void* last_seg,   void* last_ptr,
                      ptrdiff_t size_delta_bytes);

// ---------------------------------------------------------------------------
// Stage 8 cold-corner cores (see notes/cores-design.md §3.9).
//
// These do not follow the (type_ops*, storage_ops*, st_ctx, ...) contract of
// the five contiguous-storage cores above — they have only one caller family
// each and don't reuse the storage_ops vocab. They live here so a single
// consumer pulls in everything emitted from src/support.cpp.
// ---------------------------------------------------------------------------

// valarray<T>::resize / size-mismatch operator= / move-from-shorter.
//
// Destroys the `old_n` elements in [old_begin, old_begin+old_n*tops->size),
// deallocates that buffer (when old_begin != nullptr), allocates a new buffer
// for `new_n` elements via ::operator new(new_n * tops->size), and broadcasts
// `*src_value` into all new_n slots via copy_construct. When new_n == 0,
// returns nullptr and does not allocate (and src_value may be null).
//
// Trivial fast path on destroy (f_triv_destroy & f_alloc_default_life):
// skip the per-element destroy loop. Trivial fast path on broadcast
// (f_triv_copy & f_alloc_default_life): memcpy the bytes of *src_value n
// times rather than calling copy_construct per slot.
//
// valarray hard-wires std::allocator<T>, so no allocator ctx is threaded;
// the core uses tops->copy_construct with a null ctx, which is correct for
// any stateless allocator (the leaf default-constructs one).
void* valarray_resize_exact(const type_ops* tops, void* old_begin,
                            size_t old_n, size_t new_n, const void* src_value);

// path::lexically_normal / lexically_relative / lexically_proximate /
// parent_path — POSIX-char path state-machine walk.
//
// Reads `in_n` bytes from `in` (the unnormalized POSIX path) and writes the
// lexically-normalized form into [out, out + out_cap). Returns the number of
// bytes that would be written if out_cap were unbounded; the caller checks
// against out_cap to detect truncation. When out_cap == 0 (sizing pass),
// no bytes are written and only the required size is computed.
//
// Pure byte work; no allocation, no type_ops, no allocator. Output is at
// most in_n + 1 bytes (the +1 covers the empty-input "." replacement).
// Callers allocate in_n + 2 to be safe and write directly.
size_t path_lexical_normal_bytes(const char* in, size_t in_n,
                                 char* out, size_t out_cap);

} // namespace detail
} // namespace std
