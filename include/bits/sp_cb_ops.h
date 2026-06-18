// bits/sp_cb_ops.h — type-erased control-block vocabulary for shared_ptr.
//
// Replaces sp_counted_base's virtual hierarchy with a const sp_cb_ops*
// pointer stored where the vtable used to live. The four concrete control
// blocks (sp_counted_ptr_deleter, sp_counted_emplace,
// sp_unbounded_array_control_block, sp_bounded_array_control_block) each
// publish a `static inline constexpr sp_cb_ops k_ops` constant whose
// on_zero_shared / on_zero_shared_weak slots are free-function leaves
// transcribed from the existing virtual bodies.
//
// What this buys: the vtable goes away (16 B per derived instantiation
// under gcc-10, plus a null typeinfo pointer slot under -fno-rtti; see
// cores-design.md §3.7 / §9). The two function-pointer leaves remain but
// emit as free functions in their own .text.<fn> sections so
// --gc-sections drops unused ones, and they fold pairwise across template
// parameters under ICF whenever their byte content matches (e.g. two
// sp_counted_emplace<T, allocator<T>> instantiations whose
// allocator_traits::destroy resolves to the same byte sequence collapse
// to one body).
//
// The offset / size / flag slots are informational metadata reserved for
// a future "generic control-block deallocator" core (cores-design §3.7,
// stage 7 follow-on). The dispatching leaves do not read them today; they
// are populated so a downstream pass can take a const sp_cb_ops*, derive
// (deleter location, allocator location, cb size) and run the dealloc as
// typeless byte work without per-T template instantiation.
//
// No exceptions, no RTTI. Calling through a null leaf is a contract
// violation (and traps via the null call).
#pragma once
#include <cstddef>
#include <cstdint>

namespace std {
namespace detail {

// Flag bits describing the control block's shape. Diagnostic + future
// generic-dealloc-core use; the dispatching leaves do not read these.
enum sp_cb_flags : unsigned {
    // The deleter is default_delete<T> (or default_delete<T[]>). A future
    // generic on_zero_shared core uses this to skip the deleter destructor
    // and call `delete ptr` directly.
    f_sp_default_deleter   = 1u << 0,
    // The allocator is std::allocator<U> for some U. A future generic
    // on_zero_shared_weak core uses this to skip allocator->deallocate
    // and call `::operator delete(this, cb_size)` directly.
    f_sp_default_allocator = 1u << 1,
    // The owned value lives inline at ptr_offset (sp_counted_emplace and
    // the array control blocks). Distinguishes the inline-element case
    // from sp_counted_ptr_deleter, which holds an owned raw pointer.
    f_sp_inline_element    = 1u << 2,
};

// Sentinel for offset slots that don't apply to a given control block
// (e.g. sp_counted_emplace has no separate pointer field).
inline constexpr uint16_t SP_NO_OFFSET = 0xffffu;

struct sp_cb_ops {
    // Byte offsets within the control block. SP_NO_OFFSET means absent.
    // ptr_offset:     owned raw pointer (sp_counted_ptr_deleter) OR
    //                 inline element / inline array data (the emplace
    //                 and array CBs, when f_sp_inline_element is set).
    // deleter_offset: stored Deleter object (sp_counted_ptr_deleter
    //                 only).
    // alloc_offset:   stored Alloc object (all four CBs that carry one).
    uint16_t ptr_offset;
    uint16_t deleter_offset;
    uint16_t alloc_offset;
    // Size of the control block proper. For the unbounded-array CB this
    // is sizeof(CB) only — the trailing inline element bytes are sized
    // dynamically (count_) by the leaf itself.
    uint16_t cb_size;

    unsigned flags;  // sp_cb_flags bitset

    // Destroy the owned value(s). Called when the shared refcount hits zero.
    //   sp_counted_ptr_deleter:  runs the deleter, destroys the deleter.
    //   sp_counted_emplace:      destroys the inline element.
    //   array CBs:               destroys every constructed element.
    void (*on_zero_shared)(void* cb) noexcept;

    // Destroy any remaining state (the allocator) and deallocate the
    // control block itself. Called when the weak refcount hits zero.
    void (*on_zero_shared_weak)(void* cb) noexcept;
};

} // namespace detail
} // namespace std
