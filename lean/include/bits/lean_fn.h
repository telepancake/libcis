// bits/lean_fn.h — the two type-erased heap kernels behind the lean <functional>
// std::function mode-4 (heap) storage.
//
// A lean std::function is TWO words: { void* ctx; R (*invoke)(void*, Args...) }.
// Small/one-word/fn-pointer targets live inline in `ctx`; everything larger or
// non-trivially-copyable lives in a malloc block whose header is the
// detail::fn_heap_base below, followed by the payload in the same allocation:
//
//   [ copy_ctor | dtor | real_invoke | size | payoff ][ pad ][ payload ...... ]
//   ^ block base                                              ^ block + payoff
//
// The type-INDEPENDENT clone/free of that block is these two NON-TEMPLATE
// kernels; the only per-type code the heap path needs is three tiny thunks
// (copy_ctor / dtor / real_invoke) generated in <functional>.  A measurement
// (per-type heap cost 73 B routed vs 135 B self-contained; see the
// implementation report) picked this shared-kernel split over per-type
// malloc/free thunks.  One copy per binary — or one per system in liblean.*.
#pragma once
// <functional> may be included from inside a `#pragma GCC visibility
// push(hidden)` region; a hidden reference cannot bind to a definition shipped
// in liblean.a / liblean.so.  Force default visibility across this include, like
// bits/lean_sp.h, so the fn_block_* references stay bindable across the library
// boundary (and heap_invoke's address — the mode discriminator — stays unique).
#pragma GCC visibility push(default)
#include <cstddef>
#include <cstdlib>
#include <cstring>
#pragma GCC visibility pop

namespace std {
namespace detail {

// Heap-block header.  copy_ctor / dtor / real_invoke are per-type thunks;
// real_invoke is stored erased (void(*)()) and re-typed by the per-signature
// heap_invoke trampoline in <functional>.  size is sizeof the whole block,
// payoff the byte offset of the payload within it (both computed where the
// concrete callable type F is known, so the kernels stay type-erased).
struct fn_heap_base {
  void (*copy_ctor)(const void* src_payload, void* dst_payload);  // placement copy-construct one F
  void (*dtor)(void* payload);                                    // destruct one F
  void (*real_invoke)();                                          // erased R(*)(void*, Args...)
  size_t size;                                                    // sizeof(block)
  size_t payoff;                                                  // offset of payload in block
};

// ---------------------------------------------------------------------------
// The kernels — declarations only; bodies live out of line in
// lean/src/kernels.cpp (one copy per binary, or one per system as liblean.*).
// ---------------------------------------------------------------------------
#pragma GCC visibility push(default)

// Clone a heap block: malloc(size), copy the header verbatim, then copy-construct
// the payload via the block's own copy_ctor thunk.  Traps on allocation failure.
void* fn_block_clone(const void* src) noexcept;

// Destroy a heap block: run the payload's dtor thunk, then free the block.
void fn_block_free(void* blk) noexcept;

#pragma GCC visibility pop

} // namespace detail
} // namespace std
