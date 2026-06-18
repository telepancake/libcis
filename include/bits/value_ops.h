// bits/value_ops.h — type-erasure vocabulary for value-holding wrappers.
//
// A `value_ops` table describes the lifecycle of one stored type T inside a
// type-erased value container (currently `std::any`; `std::function` follows
// in stage 3). It is the sibling to `type_ops` (which is allocator-aware and
// describes a container ELEMENT); value_ops carries no allocator threading,
// adds an `f_is_small` flag (whether T fits inline in the container's small
// buffer), and carries a `type_id` slot (the address of a per-T static used
// in the no-RTTI any_cast comparison).
//
// The Action-enum switch that previously dispatched destroy/copy/move/get on
// every operation is gone: each modifier takes one indirect call to the slot
// that names it. Per-T cost falls from "switch body + four arm bodies, twice
// (small/large flavor)" to one `value_ops` constant plus four COMDAT-folded
// leaf bodies — the leaves are tiny enough to fold across distinct T at the
// byte level (small move/destroy are just `::new(p) T(...)`/`p->~T()`; large
// move is one shared symbol — see below).
//
// Storage convention for the slots: the `void*` arguments are the address of
// the CONTAINER'S storage word(s) (i.e. `&any.s_`), NOT of the stored object.
// For a small T the storage word IS the object (placement-new at that
// address); for a large T the storage word holds a `T*` pointing to the heap
// allocation. Each leaf interprets its argument(s) accordingly:
//
//   small destroy:        static_cast<T*>(storage)->~T();
//   small copy:           ::new(dst) T(*static_cast<const T*>(src));
//   small move:           ::new(dst) T(move(*static_cast<T*>(src)));
//                         static_cast<T*>(src)->~T();
//   large destroy:        delete *static_cast<T**>(storage);
//   large copy:           *static_cast<T**>(dst) = new T(**static_cast<T* const*>(src));
//   large move:           *static_cast<T**>(dst) = *static_cast<T**>(src);
//                         *static_cast<T**>(src) = nullptr;
//
// The large-move body is `void*` -> `void*` pointer transfer and is the SAME
// for every T: it ships as one shared symbol (`move_construct_large_op`) that
// every large-T value_ops aims at.
//
// Per-T cost: ~56 B for the value_ops constant + four COMDAT-folded leaves
// (~2 of which are shared across the small / large flavors). Break-even at
// ~3 distinct stored T over the today's per-T switch + arm bodies (~200 B).
//
// No allocator threading: any (and function) are not allocator-aware in
// C++20, so `f_alloc_default_life` is always set. Kept in the flags word for
// shape-consistency with `type_ops` and so future cores can read the same
// trivial-fast-path predicate without a special case.
#pragma once
#include <cstddef>
#include <new>             // placement new
#include <type_traits>
#include <utility>         // std::move

namespace std {
namespace detail {

// ------------------------------------------------------------ value-ops flags
// Diagnostic + fast-path bits. The flags word's role mirrors type_ops:
// `f_triv_X & f_alloc_default_life` is the trivial-fast-path predicate; here
// f_alloc_default_life is always set (no allocator). f_is_small lets the
// container route a copy/move to the small or large slot half of the wrapper
// without a separate vtable.
enum value_flags : unsigned {
    vf_triv_destroy        = 1u << 0,
    vf_triv_copy           = 1u << 1,  // trivially copyable (small case = memcpy)
    vf_alloc_default_life  = 1u << 2,  // always set; kept for shape parity
    vf_is_small            = 1u << 3,  // T fits in the container's inline buffer
};

// ============================================================== single table
struct value_ops {
    size_t size;
    size_t align;
    unsigned flags;

    // Storage-word ops. The void* arguments address the container's storage
    // word(s), not the stored T directly. See the header banner above for
    // the small vs large interpretation. A null slot is a contract bug at
    // this layer (the table is built per-T and every slot is reachable).
    void (*destroy)(void* storage);
    void (*copy_construct)(void* dst_storage, const void* src_storage);
    void (*move_construct)(void* dst_storage, void* src_storage);

    // Per-T identity tag. Address of a unique static (per T) — used for the
    // no-RTTI any_cast<T> comparison: matching T iff matching pointer.
    const void* type_id;
};

// ----- per-T identity tag --------------------------------------------------
// One static int per T; its address is the type_id slot. (Same shape that
// <any>'s any_imp::unique_typeinfo used to carry; pulled into this header so
// value_ops is self-contained.)
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
// One value_ops per T. Small/large is decided here; the four slots route to
// the matching leaves above. Body-level requires are not needed (any/function
// already SFINAE-gate to is_copy_constructible_v<T> on the way in).
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

// One inline constant per T. Reading this address is the per-T cost; the
// constants are picked up by the container as `const value_ops*`.
template<class T>
inline constexpr value_ops value_ops_for = make_value_ops<T>();

} // namespace detail
} // namespace std
