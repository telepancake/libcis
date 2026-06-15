// bits/type_ops.h — the type-erasure operation table and the const type_ops*
// ABI, in the library.
//
// A type_ops describes one (element type, allocator) pair to the shared,
// non-template container cores: the element size plus pointers to the leaf
// operations. The leaves route through allocator_traits::construct/destroy and
// take the owning container's allocator as `ctx`, so a custom allocator's
// construct()/destroy() is honored (for the default allocator these compile to
// placement-new / direct destructor, no overhead).
//
// A null leaf is the trivial sentinel: move_construct == null => the element is
// trivially relocatable AND the allocator has a default lifecycle, so the core
// may memcpy; destroy == null => likewise nothing to destroy. The sentinel folds
// in the allocator check, so memcpy is never used when a custom construct()/
// destroy() would have to run.
#pragma once
#include <cstddef>
#include <memory>          // allocator_traits
#include <type_traits>
#include <utility>         // declval
#include <bits/relocatable.h>

namespace std {
namespace detail {

struct type_ops {
    size_t size;
    void (*move_construct)(void* ctx, void* dst, void* src);
    void (*copy_construct)(void* ctx, void* dst, const void* src);
    void (*destroy)(void* ctx, void* p);
};

template<class T, class A> void move_construct_op(void* ctx, void* d, void* s) {
    allocator_traits<A>::construct(*static_cast<A*>(ctx), static_cast<T*>(d),
                                   static_cast<T&&>(*static_cast<T*>(s)));
}
template<class T, class A> void copy_construct_op(void* ctx, void* d, const void* s) {
    allocator_traits<A>::construct(*static_cast<A*>(ctx), static_cast<T*>(d),
                                   *static_cast<const T*>(s));
}
template<class T, class A> void destroy_op(void* ctx, void* p) {
    allocator_traits<A>::destroy(*static_cast<A*>(ctx), static_cast<T*>(p));
}

// Does the allocator customize construct/destroy for T? If so, the core must not
// bypass it with memcpy, even for a trivially-relocatable T.
template<class A, class T>
constexpr bool alloc_has_construct =
    requires(A& a, T* p) { a.construct(p, std::declval<T>()); };
template<class A, class T>
constexpr bool alloc_has_destroy =
    requires(A& a, T* p) { a.destroy(p); };
template<class A, class T>
constexpr bool alloc_default_lifecycle =
    !alloc_has_construct<A, T> && !alloc_has_destroy<A, T>;

template<class T, class A>
constexpr type_ops make_type_ops() {
    type_ops o{sizeof(T), nullptr, nullptr, nullptr};
    constexpr bool default_life = alloc_default_lifecycle<A, T>;
    if constexpr (!(is_trivially_relocatable_v<T> && default_life)) {
        if constexpr (is_move_constructible_v<T>) o.move_construct = &move_construct_op<T, A>;
        if constexpr (is_copy_constructible_v<T>) o.copy_construct = &copy_construct_op<T, A>;
    }
    if constexpr (!(is_trivially_destructible_v<T> && default_life))
        o.destroy = &destroy_op<T, A>;
    return o;
}

template<class T, class A>
inline constexpr type_ops ops_for = make_type_ops<T, A>();

// THE const type_ops* core (defined once in src/support.cpp): relocate n elements
// from src to fresh dst — memcpy when the sentinel allows, else move-construct
// each (via the allocator) then destroy the source. `ctx` is the allocator.
void core_relocate(void* dst, void* src, size_t n, const type_ops& ops, void* ctx);

// Storage axis: how a particular allocator (re)allocates, as callbacks bound to
// the allocator via ctx. `reallocate` is null unless the storage is malloc-backed
// and can grow in place (std::allocator).
struct storage_ops {
    void* (*allocate)(void* ctx, size_t n, size_t* out_cap);
    void  (*deallocate)(void* ctx, void* p, size_t cap);
    void* (*reallocate)(void* ctx, void* p, size_t n);        // null: not supported
};

// THE one grow core (defined once in src/support.cpp): grow to >= new_cap,
// preserving the first n_live elements, via the element ops + the storage ops.
// Returns the new base; writes the achieved capacity to *out_cap. ctx is the
// allocator (used by both the element leaves and the storage callbacks).
void* core_grow(void* old_base, size_t n_live, size_t old_cap, size_t new_cap,
                const type_ops& ops, const storage_ops& st, void* ctx,
                size_t* out_cap);

} // namespace detail
} // namespace std
