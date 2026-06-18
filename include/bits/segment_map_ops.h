// bits/segment_map_ops.h — type-erased segment-map vocabulary for the
// shared deque cores.
//
// Where `storage_ops` describes a single contiguous buffer, `segment_map_ops`
// describes a SEGMENTED buffer — a map of equal-capacity contiguous blocks,
// with the deque-style front/back spare around the live range.
//
// The slots:
//
//   locate(st_ctx, byte_index)
//       Locate the element at `byte_index` from the logical start of the
//       deque. Returns a byte_span repurposed as a (segment_base, in-segment
//       byte offset) pair — .data is the segment base pointer, .size is the
//       byte offset within that segment. Two pointers in a single 16-byte
//       return value; the cores use it as their "where am I right now"
//       cursor.
//
//   size_bytes(st_ctx)
//       Total bytes of one segment (block_size * sizeof(T)). The cores use
//       this as the byte-stride along a segment chain.
//
//   next_segment(st_ctx, seg)
//       Step from one segment's base pointer to the next segment's base
//       pointer in the live chain. Returns nullptr after the last segment.
//
//   push_back_segment(st_ctx)
//       Append a new (allocated) segment at the back of the map.
//
//   pop_back_segment(st_ctx)
//       Release the last segment from the back of the map.
//
//   alloc_ctx(st_ctx)
//       Mirrors storage_ops::alloc_ctx — returns the allocator instance
//       pointer when type_ops::flags say it has per-instance state, else
//       nullptr. Fetched lazily by the cores via tops->flags & f_alloc_ctx.
//
// The iterator at this layer is a (segment-base, in-segment byte-pointer)
// pair carried as two void*s through the core signature. The container
// converts at the boundary — the cores never see deque_iterator.
#pragma once
#include <cstddef>
#include <bits/type_ops.h>
#include <bits/storage_ops.h>   // byte_span

namespace std {
namespace detail {

struct segment_map_ops {
    // (st_ctx, byte_index_from_logical_start) -> { segment_base, byte_offset_within_segment }
    byte_span (*locate)(void* st_ctx, size_t byte_index);
    // Bytes per segment (block_size * sizeof(T)).
    size_t    (*size_bytes)(void* st_ctx);
    // Next segment base after `seg`, or nullptr at end of live chain.
    void*     (*next_segment)(void* st_ctx, void* seg);
    // Append / drop a back segment of the live chain.
    void      (*push_back_segment)(void* st_ctx);
    void      (*pop_back_segment) (void* st_ctx);
    // Allocator instance pointer when type_ops::flags say it has state,
    // else nullptr (same lazy-fetch contract as storage_ops::alloc_ctx).
    void*     (*alloc_ctx)        (void* st_ctx);
};

} // namespace detail
} // namespace std
