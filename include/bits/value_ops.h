// bits/value_ops.h — type-erasure vocab tables for value-holding wrappers.
//
// Two siblings live here, one per wrapper family:
//
//   value_ops          — single-T lifecycle + identity for <any>.
//   callable_ops<Sig>  — per-signature lifecycle + invoke for <functional>'s
//                        function<Sig>.
//
// Both are sized in the SAME shape as type_ops (the container/element vocab):
//   - flags / size / align prefix carrying triviality bits read by the cores.
//   - per-leaf function pointers (destroy/copy/move) addressable from a
//     typeless helper in src/support.cpp.
//   - identity / invoke slots appended after the lifecycle prefix.
//
// Storage convention for the lifecycle slots: the `void*` arguments address
// the CONTAINER'S storage word(s) (i.e. `&any.s_`, `&fn.buf_`), NOT the held
// object. For a SMALL stored type the storage word IS the object (placement-
// new at that address); for a LARGE stored type the storage word holds a
// pointer to the heap allocation.
//
// Neither vocab carries allocator threading: <any> and <function> are not
// allocator-aware in C++20. f_alloc_default_life is always set (in value_ops)
// for shape-consistency with type_ops; callable_ops simply has no allocator
// axis. Both consequently use their own f_* enumerators (`vf_*` and `vo_f_*`)
// prefixed to avoid collision with type_ops's `f_*` in the same std::detail
// namespace.
#pragma once
#include <cstddef>
#include <new>             // placement new
#include <type_traits>
#include <utility>         // std::move / std::forward
#include <bits/relocatable.h>

namespace std {
namespace detail {

// =========================================================================
// value_ops — single-T vocab for std::any
// =========================================================================
//
// The Action-enum switch that previously dispatched destroy/copy/move/get on
// every <any> operation is gone: each modifier takes one indirect call to
// the slot that names it. Per-T cost falls from "switch body + four arm
// bodies, twice (small/large flavor)" to one `value_ops` constant plus four
// COMDAT-folded leaf bodies — the leaves are tiny enough to fold across
// distinct T at the byte level (small move/destroy are just
// `::new(p) T(...)`/`p->~T()`; large move is one shared symbol —
// `move_construct_large_op` — that every large-T value_ops aims at).
//
// Per-T cost: ~56 B for the value_ops constant + four COMDAT-folded leaves
// (~2 of which are shared across the small / large flavors). Break-even at
// ~3 distinct stored T over the prior per-T switch + arm bodies (~200 B).

enum value_flags : unsigned {
    vf_triv_destroy        = 1u << 0,
    vf_triv_copy           = 1u << 1,  // trivially copyable (small case = memcpy)
    vf_alloc_default_life  = 1u << 2,  // always set; kept for shape parity
    vf_is_small            = 1u << 3,  // T fits in the container's inline buffer
};

struct value_ops {
    size_t size;
    size_t align;
    unsigned flags;

    // Storage-word ops. The void* arguments address the container's storage
    // word(s), not the stored T directly. A null slot is a contract bug at
    // this layer (the table is built per-T and every slot is reachable).
    void (*destroy)(void* storage);
    void (*copy_construct)(void* dst_storage, const void* src_storage);
    void (*move_construct)(void* dst_storage, void* src_storage);

    // Per-T identity tag. Address of a unique static (per T) — used for the
    // no-RTTI any_cast<T> comparison: matching T iff matching pointer.
    const void* type_id;
};

// ----- per-T identity tag --------------------------------------------------
// One static int per T; its address is the type_id slot. Same shape that
// <any>'s any_imp::unique_typeinfo used to carry; pulled into this header so
// value_ops is self-contained.
template<class T>
struct value_type_id {
    static constexpr int id = 0;
};

// ----- small/large policy --------------------------------------------------
// Mirrors <any>'s small-buffer policy literally: a T is "small" when it fits
// in 3 ptr-aligned words AND has a noexcept move-ctor. The container fixes
// the inline-buffer size; this header just publishes the predicate so the
// per-T table picks the right leaf bodies.
inline constexpr size_t value_small_size      = 3 * sizeof(void*);
inline constexpr size_t value_small_alignment = alignof(void*);

template<class T>
inline constexpr bool value_is_small =
    sizeof(T)  <= value_small_size &&
    alignof(T) <= value_small_alignment &&
    is_nothrow_move_constructible_v<T>;

// ----- SMALL leaves --------------------------------------------------------
// T is at the storage word's address (placement-new'd in place).
template<class T> void destroy_small_op(void* storage) {
    static_cast<T*>(storage)->~T();
}
template<class T> void copy_construct_small_op(void* dst, const void* src) {
    ::new (dst) T(*static_cast<const T*>(src));
}
template<class T> void move_construct_small_op(void* dst, void* src) {
    ::new (dst) T(std::move(*static_cast<T*>(src)));
    static_cast<T*>(src)->~T();
}

// ----- LARGE leaves --------------------------------------------------------
// Storage word holds a T*. destroy/copy run T's dtor / copy-ctor on the
// heap object; move is pure pointer transfer and is shared across all T.
template<class T> void destroy_large_op(void* storage) {
    delete *static_cast<T**>(storage);
}
template<class T> void copy_construct_large_op(void* dst, const void* src) {
    *static_cast<T**>(dst) = new T(**static_cast<T* const*>(src));
}
// One shared symbol across every large T: pointer-word transfer + nullify.
// Defined inline (header-only); the linker folds this to a single body.
inline void move_construct_large_op(void* dst, void* src) {
    void** d = static_cast<void**>(dst);
    void** s = static_cast<void**>(src);
    *d = *s;
    *s = nullptr;
}

// ----- table builder -------------------------------------------------------
template<class T>
constexpr value_ops make_value_ops() {
    value_ops o{sizeof(T), alignof(T), 0u,
                nullptr, nullptr, nullptr, nullptr};

    if constexpr (is_trivially_destructible_v<T>) o.flags |= vf_triv_destroy;
    if constexpr (is_trivially_copyable_v<T>)     o.flags |= vf_triv_copy;
    o.flags |= vf_alloc_default_life;             // any has no allocator
    if constexpr (value_is_small<T>)              o.flags |= vf_is_small;

    if constexpr (value_is_small<T>) {
        o.destroy        = &destroy_small_op<T>;
        o.copy_construct = &copy_construct_small_op<T>;
        o.move_construct = &move_construct_small_op<T>;
    } else {
        o.destroy        = &destroy_large_op<T>;
        o.copy_construct = &copy_construct_large_op<T>;
        o.move_construct = &move_construct_large_op;   // shared, non-template
    }

    o.type_id = &value_type_id<T>::id;
    return o;
}

template<class T>
inline constexpr value_ops value_ops_for = make_value_ops<T>();


// =========================================================================
// callable_ops<Sig> — per-signature vocab for std::function
// =========================================================================

// triviality + layout flags for callable_ops. f_is_small is the small-buffer-
// optimisation discriminator: the wrapper stores the held value inline at
// &buf_[0] when set, and on the heap with the pointer cached in
// *(void**)&buf_[0] when clear.
//
// Prefixed `vo_` to avoid colliding with type_ops's `f_triv_*` enumerators
// in the same std::detail namespace. callable_ops has no allocator axis, so
// its flag set is its own.
enum value_ops_flags : unsigned {
    vo_f_triv_destroy = 1u << 0,  // trivially destructible (destroy elided)
    vo_f_triv_copy    = 1u << 1,  // trivially copyable (copy_construct == memcpy)
    vo_f_triv_reloc   = 1u << 2,  // trivially relocatable (move_construct == memcpy)
    vo_f_is_small     = 1u << 3,  // inline-fits-and-nothrow-moveable into 3 ptrs
};

// callable_ops_base — the signature-INDEPENDENT prefix of callable_ops<Sig>.
// Defined as its own struct so the typeless support.cpp helpers
// (func_copy_into / func_move_into / func_destroy_held) can take a
// `const callable_ops_base*` without a per-signature template — the layout
// of the prefix is identical across signatures and a
// `reinterpret_cast<const callable_ops_base*>(&callable_ops_for<...>)` is
// well-formed (standard-layout struct, first field at offset 0).
struct callable_ops_base {
    unsigned flags;
    size_t   size;
    size_t   align;
    void   (*destroy)(void* p);
    void   (*copy_construct)(void* dst, const void* src);
    void   (*move_construct)(void* dst, void* src);
};

template<class Sig> struct callable_ops;   // primary undefined; only Sig=R(A...) is meaningful

template<class R, class... A>
struct callable_ops<R(A...)> {
    // Lifecycle prefix — MUST match callable_ops_base layout exactly.
    unsigned flags;
    size_t   size;
    size_t   align;
    void   (*destroy)(void* p);
    void   (*copy_construct)(void* dst, const void* src);
    void   (*move_construct)(void* dst, void* src);
    // Per-signature: invoke the held callable.
    R      (*invoke)(void* p, A&&... args);
};

// Verify the layout assumption the cast relies on.
static_assert(offsetof(callable_ops<void()>, flags)          == offsetof(callable_ops_base, flags));
static_assert(offsetof(callable_ops<void()>, size)           == offsetof(callable_ops_base, size));
static_assert(offsetof(callable_ops<void()>, align)          == offsetof(callable_ops_base, align));
static_assert(offsetof(callable_ops<void()>, destroy)        == offsetof(callable_ops_base, destroy));
static_assert(offsetof(callable_ops<void()>, copy_construct) == offsetof(callable_ops_base, copy_construct));
static_assert(offsetof(callable_ops<void()>, move_construct) == offsetof(callable_ops_base, move_construct));

// ---- per-F leaves -----------------------------------------------------------
template<class F> void func_destroy_op(void* p) {
    static_cast<F*>(p)->~F();
}
template<class F> void func_copy_construct_op(void* dst, const void* src) {
    ::new (dst) F(*static_cast<const F*>(src));
}
template<class F> void func_move_construct_op(void* dst, void* src) {
    ::new (dst) F(static_cast<F&&>(*static_cast<F*>(src)));
}

// invoke leaf: forwarding form. The wrapper has already validated
// is_invocable_r_v<R, F&, A...> at the call-site that built the ops table.
// We use detail::invoke_impl (declared at the top of <functional> by the time
// consumers instantiate this template) rather than std::invoke because
// value_ops.h is included from <functional> BEFORE std::invoke is defined.
template<class F, class R, class... A>
R func_invoke_op(void* p, A&&... args) {
    if constexpr (is_void_v<R>) {
        (void)detail::invoke_impl(*static_cast<F*>(p), std::forward<A>(args)...);
    } else {
        return detail::invoke_impl(*static_cast<F*>(p), std::forward<A>(args)...);
    }
}

// ---- make_callable_ops<F, Sig> ----------------------------------------------
inline constexpr size_t cb_small_buffer_size      = 3 * sizeof(void*);
inline constexpr size_t cb_small_buffer_alignment = alignof(void*);

template<class F>
inline constexpr bool callable_is_small =
    sizeof(F)  <= cb_small_buffer_size &&
    alignof(F) <= cb_small_buffer_alignment &&
    is_nothrow_move_constructible_v<F>;

template<class F, class Sig> struct make_callable_ops_impl;

template<class F, class R, class... A>
struct make_callable_ops_impl<F, R(A...)> {
    static constexpr callable_ops<R(A...)> value() {
        callable_ops<R(A...)> o{};
        o.size  = sizeof(F);
        o.align = alignof(F);
        unsigned flg = 0;
        if constexpr (is_trivially_destructible_v<F>)  flg |= vo_f_triv_destroy;
        if constexpr (is_trivially_copyable_v<F>)      flg |= vo_f_triv_copy;
        if constexpr (is_trivially_relocatable_v<F>)   flg |= vo_f_triv_reloc;
        if constexpr (callable_is_small<F>)            flg |= vo_f_is_small;
        o.flags = flg;

        if constexpr (!is_trivially_destructible_v<F>)
            o.destroy = &func_destroy_op<F>;
        if constexpr (requires(F* p, const F& s) { ::new (p) F(s); })
            if constexpr (!is_trivially_copyable_v<F>)
                o.copy_construct = &func_copy_construct_op<F>;
        if constexpr (requires(F* p, F&& s) { ::new (p) F(static_cast<F&&>(s)); })
            if constexpr (!is_trivially_relocatable_v<F>)
                o.move_construct = &func_move_construct_op<F>;

        o.invoke = &func_invoke_op<F, R, A...>;
        return o;
    }
};

template<class F, class Sig>
inline constexpr callable_ops<Sig> callable_ops_for =
    make_callable_ops_impl<F, Sig>::value();

// -------------------------------------- typeless support.cpp lifecycle cores
// Declared here, defined in src/support.cpp. They take a callable_ops_base*
// (sig-independent prefix) plus the wrapper's inline buffer; the buffer is
// the wrapper's `char buf_[3*sizeof(void*)]`. Each helper handles the
// small-vs-heap discrimination + the trivial fast path off ops->flags, so
// the wrapper body has one call per modifier.
//
// Storage convention:
//   small (vo_f_is_small): held object lives in-place at buf_[0..size).
//                          copies/moves use the per-F leaves or the memcpy
//                          fast path.
//   large (!vo_f_is_small): held object lives on the heap. The pointer is
//                           stored at *(void**)&buf_[0]; the leaves operate
//                           on that pointer. Copies allocate a fresh heap
//                           object; moves transfer ownership of the pointer
//                           (no leaf call) and clear the source slot.

void func_copy_into(const callable_ops_base* ops, void* dst_buf, const void* src_buf);
void func_move_into(const callable_ops_base* ops, void* dst_buf, void* src_buf);
void func_destroy_held(const callable_ops_base* ops, void* buf);

} // namespace detail
} // namespace std
