// bits/future_cb_ops.h — vocab table that replaces the assoc_sub_state vtable.
//
// The <future> shared-state hierarchy (assoc_sub_state base + assoc_state<R>,
// assoc_state_alloc<R, Alloc>, deferred_assoc_state<R, F>, async_assoc_state<R, F>
// derived classes) used a virtual on_zero() and a virtual execute(). Each
// derived instantiation emitted a vtable + the virtual bodies; the bodies fold
// pairwise across (R, F, Alloc) at the byte level except for the vtable pointer
// keyed on the concrete subclass.
//
// future_callback_ops replaces the vtable: assoc_sub_state stores a
// `const future_callback_ops*` instead of a vptr; on_zero/execute dispatch
// through the table. Per stages-7/9 design (notes/cores-design.md §3.7-§3.8),
// the per-flavor table is a constant ~32 B and the bodies fold across template
// parameters under COMDAT.
//
// invoke() is signature-erased: <future>'s callbacks are *always* R(void) by
// the time they reach the shared state — packaged_task binds arguments at
// operator() time, async binds in async_func at construction. So one slot
// suffices, no per-Sig parameterization.
#pragma once
#include <cstddef>

namespace std {
namespace detail {

// Flag bits for future_callback_ops::flags. Diagnostic + correctness today;
// `f_fcb_alloc` records that destroy_value will dispatch through a user
// allocator's deallocate (rather than ::operator delete). `f_fcb_async` and
// `f_fcb_deferred` record which spawning path produced the state, useful for
// introspection and for keeping the invoke / destroy_value bodies symmetric
// across instantiations.
enum future_cb_flags : unsigned {
    f_fcb_async    = 1u << 0,  // destroy_value must wait for a detached thread
    f_fcb_deferred = 1u << 1,  // invoke() runs a user callback; execute() body present
    f_fcb_alloc    = 1u << 2,  // state was allocated through a user allocator
};

// One vocab table per concrete (R, Alloc, F) flavor of assoc_*_state. The
// table replaces a vtable that would have been keyed on the same flavor.
//
// destroy_value(state) — invoked from release_shared() when the shared count
//   reaches zero. The callee knows the concrete subclass, so it does the
//   complete on_zero job: destroy the stored value (if `state_ & constructed`),
//   run the subclass destructor, and deallocate the state's storage via the
//   matching mechanism (::delete, allocator_traits::deallocate, or
//   wait-then-delete for async).
//
// invoke(state) — invoked from sub_wait() (deferred path) and from the async
//   thread body. The callee knows the concrete subclass and stored callable;
//   it runs the callable and stores the result/exception/void into the state.
//   For plain promise-backed states (no callable) the slot points at a no-op
//   leaf, matching the previous virtual `execute() {}` default.
struct future_callback_ops {
    size_t value_size;
    size_t value_align;
    unsigned flags;
    void (*destroy_value)(void* state);
    void (*invoke)(void* state);
};

// Shared no-op leaf for plain assoc_sub_state / assoc_state<R> instances whose
// only callback is the promise side (set_value), not a stored callable. One
// symbol across all such flavors.
inline void future_cb_invoke_noop(void*) {}

} // namespace detail
} // namespace std
