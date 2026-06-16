// bits/type_ops.h — the type-erasure operation tables and the const-ops* ABI,
// in the library. Two tables, by arity of the *types* involved:
//
//   type_ops        — ops on a single element type T (lifecycle, ordering,
//                     equality, hash, swap).
//   cross_ops       — ops genuinely between two types T and U (construct/assign
//                     a T from a U, compare/equal a T against a U, cast U->T).
//
// A set function pointer means "non-trivial / customized — call me"; a null
// pointer means "trivial OR invalid", and the core handles the trivial case
// inline (memcpy / memmove / nothing). The `flags` bitset records *why* a
// pointer is null (which triviality), for introspection and asserts.
//
// Allocator-awareness: the lifecycle leaves (default/copy/move construct,
// destroy) take the owning container's allocator as `ctx` and route through
// allocator_traits::construct/destroy, so a custom allocator's construct()/
// destroy() is honored (for std::allocator they fold to placement-new / direct
// destructor, no overhead). The trivial-relocate sentinel (null move_construct)
// is therefore DOUBLY gated: it fires only when the element is trivially
// relocatable AND the allocator has a default lifecycle, so a counting/
// instrumenting allocator is never bypassed by a memcpy. The value ops
// (assign/swap/compare/equal/hash) act on already-constructed objects and are
// not allocator-mediated, so they take no ctx.
#pragma once
#include <cstddef>
#include <new>             // placement new (default-lifecycle leaves)
#include <memory>          // allocator_traits
#include <type_traits>
#include <utility>         // declval, swap
#include <bits/hash.h>     // std::hash (lightweight; no <vector>/<functional> cycle)
#include <bits/relocatable.h>

namespace std {
namespace detail {

// ------------------------------------------------ triviality (diagnostic only)
// Records why an op pointer is null. The core dispatches on null-vs-set, not on
// these; they are for introspection / static_assert / debugging, zero runtime.
enum ops_flags : unsigned {
    f_triv_default = 1u << 0,  // trivially default-constructible
    f_triv_copy    = 1u << 1,  // trivially copyable (copy/move/assign == memcpy)
    f_triv_reloc   = 1u << 2,  // trivially relocatable (grow == memmove)
    f_triv_destroy = 1u << 3,  // trivially destructible (destroy == nothing)
    // LOAD-BEARING (unlike the f_triv_* above): set iff the allocator is
    // STATEFUL, so the lifecycle leaves need its specific instance as ctx. The
    // realloc_op / core reads this to decide whether to materialize the allocator
    // instance (from the container ctx) before relocating; when clear (stateless
    // / type-only), the leaves synthesize one and run with a null ctx.
    // Independent of lifecycle.
    f_alloc_ctx    = 1u << 4,
};

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

// Is the allocator type-only (stateless), so a leaf can make one on the spot and
// the core need not pass an instance pointer? True iff all instances are
// interchangeable (is_always_equal) AND one can be default-constructed. A
// stateful allocator (false) must have its specific instance handed to the
// leaves. This is orthogonal to lifecycle customization above.
template<class A> using alloc_always_equal_t = typename allocator_traits<A>::is_always_equal;
template<class A>
constexpr bool alloc_stateless =
    alloc_always_equal_t<A>::value && is_default_constructible_v<A>;

// =====================================================================
// Single-type table
// =====================================================================
struct type_ops {
    size_t size;
    size_t align;
    unsigned flags;

    // allocator-mediated lifecycle (ctx == the allocator)
    void (*default_construct)(void* ctx, void* p);              // null: trivial+default-life or invalid
    void (*destroy)(void* ctx, void* p);                        // null: trivial+default-life
    void (*copy_construct)(void* ctx, void* dst, const void* s);// null: trivial+default-life or move-only
    void (*move_construct)(void* ctx, void* dst, void* src);    // null: triv-reloc+default-life or immovable

    // value ops on constructed objects (no allocator)
    void (*copy_assign)(void* dst, const void* src);            // null: trivial or not copy-assignable
    void (*move_assign)(void* dst, void* src);                  // null: trivial or not move-assignable
    void (*swap)(void* a, void* b);                             // null: trivial or not swappable
    int  (*compare)(const void* a, const void* b);             // 3-way; null: not ordered
    bool (*equal)(const void* a, const void* b);               // null: not equality-comparable
    size_t (*hash)(const void* p);                              // null: not hashable
};

// ---- single-type leaf ops (instantiated only when named in make_type_ops) ----
// Lifecycle leaves route through allocator_traits, which itself folds to
// placement-new / direct destructor when the allocator doesn't customize them.
// The only thing the leaf decides is WHERE the allocator comes from: a stateless
// allocator is synthesized on the spot (ctx ignored, the caller passes null —
// f_alloc_ctx clear); a stateful one is read from ctx (its instance, which the
// realloc_op / core fetches from the container — f_alloc_ctx set). So the common
// case carries no allocator pointer, while a stateful/customizing allocator is
// honored.
template<class T, class A> void default_construct_op(void* ctx, void* p) {
    if constexpr (alloc_stateless<A>) { A a{}; allocator_traits<A>::construct(a, static_cast<T*>(p)); }
    else allocator_traits<A>::construct(*static_cast<A*>(ctx), static_cast<T*>(p));
}
template<class T, class A> void destroy_op(void* ctx, void* p) {
    if constexpr (alloc_stateless<A>) { A a{}; allocator_traits<A>::destroy(a, static_cast<T*>(p)); }
    else allocator_traits<A>::destroy(*static_cast<A*>(ctx), static_cast<T*>(p));
}
template<class T, class A> void copy_construct_op(void* ctx, void* d, const void* s) {
    if constexpr (alloc_stateless<A>) { A a{}; allocator_traits<A>::construct(a, static_cast<T*>(d), *static_cast<const T*>(s)); }
    else allocator_traits<A>::construct(*static_cast<A*>(ctx), static_cast<T*>(d),
                                        *static_cast<const T*>(s));
}
template<class T, class A> void move_construct_op(void* ctx, void* d, void* s) {
    if constexpr (alloc_stateless<A>) { A a{}; allocator_traits<A>::construct(a, static_cast<T*>(d), static_cast<T&&>(*static_cast<T*>(s))); }
    else allocator_traits<A>::construct(*static_cast<A*>(ctx), static_cast<T*>(d),
                                        static_cast<T&&>(*static_cast<T*>(s)));
}
// value ops:
template<class T> void copy_assign_op(void* d, const void* s) {
    *static_cast<T*>(d) = *static_cast<const T*>(s);
}
template<class T> void move_assign_op(void* d, void* s) {
    *static_cast<T*>(d) = static_cast<T&&>(*static_cast<T*>(s));
}
template<class T> void swap_op(void* a, void* b) {
    using std::swap;
    swap(*static_cast<T*>(a), *static_cast<T*>(b));
}
template<class T> int compare_op(const void* a, const void* b) {
    const T& x = *static_cast<const T*>(a);
    const T& y = *static_cast<const T*>(b);
    return x < y ? -1 : (y < x ? 1 : 0);
}
template<class T> bool equal_op(const void* a, const void* b) {
    return *static_cast<const T*>(a) == *static_cast<const T*>(b);
}
template<class T> size_t hash_op(const void* p) {
    return std::hash<T>{}(*static_cast<const T*>(p));
}

template<class T, class A>
constexpr type_ops make_type_ops() {
    type_ops o{sizeof(T), alignof(T), 0u,
               nullptr, nullptr, nullptr, nullptr,   // default_construct, destroy, copy_construct, move_construct
               nullptr, nullptr, nullptr,            // copy_assign, move_assign, swap
               nullptr, nullptr, nullptr};           // compare, equal, hash

    constexpr bool default_life = alloc_default_lifecycle<A, T>;

    if constexpr (is_trivially_default_constructible_v<T>) o.flags |= f_triv_default;
    if constexpr (is_trivially_copyable_v<T>)              o.flags |= f_triv_copy;
    if constexpr (is_trivially_relocatable_v<T>)           o.flags |= f_triv_reloc;
    if constexpr (is_trivially_destructible_v<T>)          o.flags |= f_triv_destroy;
    // the only load-bearing flag: leaves need the allocator instance iff it is
    // stateful (a stateless one is synthesized in the leaf). Independent of the
    // lifecycle/sentinel decision below.
    if constexpr (!alloc_stateless<A>)                     o.flags |= f_alloc_ctx;

    // Allocator-mediated lifecycle: a leaf is needed when the op is non-trivial
    // OR the allocator customizes it. Null only when trivial AND default
    // lifecycle, so the core's memcpy/skip path can never bypass a custom
    // construct()/destroy().
    if constexpr (is_default_constructible_v<T>)
        if constexpr (!(is_trivially_default_constructible_v<T> && default_life))
            o.default_construct = &default_construct_op<T, A>;
    if constexpr (!(is_trivially_destructible_v<T> && default_life))
        o.destroy = &destroy_op<T, A>;
    if constexpr (is_copy_constructible_v<T>)
        if constexpr (!(is_trivially_copyable_v<T> && default_life))
            o.copy_construct = &copy_construct_op<T, A>;
    if constexpr (is_move_constructible_v<T>)
        if constexpr (!(is_trivially_relocatable_v<T> && default_life))
            o.move_construct = &move_construct_op<T, A>;

    // Value ops: not allocator-mediated; null when trivial (the core does the
    // memcpy/nop inline) or when the operation is ill-formed for T.
    if constexpr (is_copy_assignable_v<T> && !is_trivially_copyable_v<T>)
        o.copy_assign = &copy_assign_op<T>;
    if constexpr (is_move_assignable_v<T> && !is_trivially_copyable_v<T>)
        o.move_assign = &move_assign_op<T>;
    if constexpr (is_swappable_v<T> && !is_trivially_copyable_v<T>)
        o.swap = &swap_op<T>;
    if constexpr (requires(const T& a, const T& b) { a < b; })
        o.compare = &compare_op<T>;
    if constexpr (requires(const T& a, const T& b) { a == b; })
        o.equal = &equal_op<T>;
    if constexpr (requires(const T& a) { std::hash<T>{}(a); })
        o.hash = &hash_op<T>;

    return o;
}

template<class T, class A>
inline constexpr type_ops ops_for = make_type_ops<T, A>();

// =====================================================================
// Two-type table: ops between a T and a U
// =====================================================================
// Embeds pointers to the single-type tables for T (first) and U (second), so a
// two-type generic algorithm receives ONE ops pointer instead of three. The
// cross construct/assign leaves use placement-new / direct assignment (they are
// for algorithm-level cross-type work); a container that needs an
// allocator-mediated cross-construct routes through its own single-type
// construct leaf instead.
struct cross_ops {
    const type_ops* first;                       // single-type ops for T
    const type_ops* second;                      // single-type ops for U
    void (*construct_from)(void*, const void*);  // ::new(T) T(U)   null: not constructible
    void (*assign_from)(void*, const void*);     // T = U           null: not assignable
    int  (*compare)(const void*, const void*);   // T <=> U 3-way   null: not comparable
    bool (*equal)(const void*, const void*);     // T == U          null: not equality-comparable
    void (*cast)(void*, const void*);            // ::new(T) T(static_cast<T>(U)) null: cast ill-formed
};

template<class T, class U> void construct_from_op(void* d, const void* s) {
    ::new (d) T(*static_cast<const U*>(s));
}
template<class T, class U> void assign_from_op(void* d, const void* s) {
    *static_cast<T*>(d) = *static_cast<const U*>(s);
}
template<class T, class U> int compare_cross_op(const void* a, const void* b) {
    const T& x = *static_cast<const T*>(a);
    const U& y = *static_cast<const U*>(b);
    return x < y ? -1 : (y < x ? 1 : 0);
}
template<class T, class U> bool equal_cross_op(const void* a, const void* b) {
    return *static_cast<const T*>(a) == *static_cast<const U*>(b);
}
template<class T, class U> void cast_op(void* d, const void* s) {
    ::new (d) T(static_cast<T>(*static_cast<const U*>(s)));
}

// cross_ops embeds single-type tables; default-allocator instances suffice for
// the embedded pointers (the cross leaves do not allocate).
template<class T, class U>
constexpr cross_ops make_cross_ops() {
    cross_ops o{&ops_for<T, allocator<T>>, &ops_for<U, allocator<U>>,
                nullptr, nullptr, nullptr, nullptr, nullptr};
    if constexpr (is_constructible_v<T, const U&>)
        o.construct_from = &construct_from_op<T, U>;
    if constexpr (is_assignable_v<T&, const U&>)
        o.assign_from = &assign_from_op<T, U>;
    if constexpr (requires(const T& t, const U& u) { t < u; u < t; })
        o.compare = &compare_cross_op<T, U>;
    if constexpr (requires(const T& t, const U& u) { t == u; })
        o.equal = &equal_cross_op<T, U>;
    if constexpr (requires(const U& u) { static_cast<T>(u); })
        o.cast = &cast_op<T, U>;
    return o;
}

template<class T, class U>
inline constexpr cross_ops cross_for = make_cross_ops<T, U>();

// =====================================================================
// Storage axis: realloc_op
// =====================================================================
// The third axis, orthogonal to the element ops above and deliberately ignorant
// of them: raw STORAGE only. A realloc-style function bound to the CONTAINER
// through one opaque `void* ctx` (its `this`), so a non-template core takes one
// `realloc_op` + that ctx and never names the container or allocator type. It
// resizes a malloc-backed buffer and PRESERVES its bytes — nothing more. It is
// not the allocator's business to know the element type or to construct/move/
// destroy elements, so it takes no `type_ops`.
//
//   realloc_op(ctx, cur, &size) -> new_base
//
//   ctx     the owning container (its `this`), so the function can grow in place,
//           use an inline/small-buffer, or reach its allocator.
//   cur     current buffer base, or null if there is none yet.
//   size    in/out, in BYTES: on entry the requested size; on return the actual
//           size of the (possibly larger) buffer. The container converts element
//           count * element size itself — the allocator works in bytes.
//   returns the (possibly moved) base, first min(old,new) bytes preserved. MAY
//           equal `cur` (grew in place / SSO); if it moved, the OLD buffer is
//           already freed and the caller must NOT free it. Allocation failure
//           does not return null — it traps (no exceptions).
//
// Because it only copies BYTES, a container uses it solely for the trivially-
// relocatable grow fast path. For non-relocatable elements the container does
// its own allocate + element-wise relocate (via the element ops) + free; that
// relocation is the container's concern, not the allocator's.
using realloc_op = void* (*)(void* ctx, void* cur, size_t* size);

} // namespace detail
} // namespace std
