// bits/type_ops.h — the type-erasure operation tables and the const-ops* ABI,
// in the library. Two tables, by arity of the *types* involved:
//
//   type_ops        — ops on a single element type T (lifecycle, ordering,
//                     equality, hash, swap).
//   cross_ops       — ops genuinely between two types T and U (construct/assign
//                     a T from a U, compare/equal a T against a U, cast U->T).
//
// Each function pointer is set when the operation is non-trivial (or allocator-
// customized) for the type, and null when the operation is trivial or ill-formed
// for it. The `flags` bitset records which triviality made a pointer null.
//
// The lifecycle leaves (default/copy/move construct, destroy) take an allocator
// as `ctx` and route through allocator_traits::construct/destroy, so a custom
// allocator's construct()/destroy() is honored (for std::allocator these fold to
// placement-new / direct destructor). Null move_construct is doubly gated:
// present only when the element is trivially relocatable AND the allocator has a
// default lifecycle. The value ops (assign/swap/compare/equal/hash) act on
// already-constructed objects, are not allocator-mediated, and take no ctx.
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
// Record which triviality made an op pointer null. Diagnostic only, not part of
// the dispatch (that is null-vs-set on the pointer itself); for introspection /
// static_assert / debugging — EXCEPT f_alloc_ctx and f_alloc_default_life
// which the cores REQUIRE for correctness.
//
// Consumer-side trivial-fast-path check is `flags & f_triv_X` AND
// `flags & f_alloc_default_life`: both must hold to memcpy/memset/skip in
// place of dispatching through the leaf. The leaf pointer being null is NOT
// a "trivial" signal; it means the leaf was elided by MASK or is ill-formed
// for T, and calling it when flags say non-trivial is a bug (and crashes
// diagnostically).
enum ops_flags : unsigned {
    f_triv_default = 1u << 0,  // trivially default-constructible
    f_triv_copy    = 1u << 1,  // trivially copyable (copy/move/assign == memcpy)
    f_triv_reloc   = 1u << 2,  // trivially relocatable (grow == memmove)
    f_triv_destroy = 1u << 3,  // trivially destructible (destroy == nothing)
    // LOAD-BEARING: set iff the allocator is STATEFUL — i.e. the lifecycle
    // leaves require its specific instance as their ctx. When clear (stateless
    // / type-only), the leaves synthesize an allocator and ignore ctx.
    f_alloc_ctx           = 1u << 4,
    // LOAD-BEARING: set iff the allocator's construct() and destroy() are the
    // default delegations to T's own ctors/dtor (so memcpy / memset / skip is
    // semantically equivalent to running the leaf). Cleared when the allocator
    // customizes either, in which case the trivial fast path is unsafe and the
    // core MUST dispatch through the leaf. Together with the matching
    // f_triv_X flag is the only correct consumer-side trivial check.
    f_alloc_default_life  = 1u << 5,
};

// Whether the allocator customizes construct/destroy for T. When it does,
// alloc_default_lifecycle is false and make_type_ops leaves the lifecycle leaves
// set (non-null) even for a trivially-relocatable T.
template<class A, class T>
constexpr bool alloc_has_construct =
    requires(A& a, T* p) { a.construct(p, std::declval<T>()); };
template<class A, class T>
constexpr bool alloc_has_destroy =
    requires(A& a, T* p) { a.destroy(p); };
template<class A, class T>
constexpr bool alloc_default_lifecycle =
    !alloc_has_construct<A, T> && !alloc_has_destroy<A, T>;

// Whether the allocator is stateless (type-only): true iff all instances are
// interchangeable (is_always_equal) AND one is default-constructible. When true,
// a leaf default-constructs an allocator rather than reading an instance from
// ctx. Orthogonal to lifecycle customization above.
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

    // allocator-mediated lifecycle (ctx == the allocator).
    // Each leaf is non-null iff:
    //   (a) the consumer's MASK bit asks for it, AND
    //   (b) it is installable for T (the body-level `requires` gate passes), AND
    //   (c) the op is trivial+default-life elision did not kick in.
    // The consumer is responsible for taking the trivial fast path before
    // dispatch: check (flags & f_triv_X) & (flags & f_alloc_default_life).
    // Calling a null leaf is a contract violation and traps.
    void (*default_construct)(void* ctx, void* p);
    void (*destroy)(void* ctx, void* p);
    void (*copy_construct)(void* ctx, void* dst, const void* s);
    void (*move_construct)(void* ctx, void* dst, void* src);

    // value ops on constructed objects (no allocator). Same null contract.
    void (*copy_assign)(void* dst, const void* src);
    void (*move_assign)(void* dst, void* src);
    void (*swap)(void* a, void* b);
    int  (*compare)(const void* a, const void* b);
    bool (*equal)(const void* a, const void* b);
    size_t (*hash)(const void* p);                              // null: not hashable
};

// ---- single-type leaf ops (instantiated only when named in make_type_ops) ----
// Lifecycle leaves route through allocator_traits (which folds to placement-new /
// direct destructor when the allocator doesn't customize them). For a stateless
// allocator the leaf default-constructs one and ignores ctx; for a stateful
// allocator it reads the instance from ctx. (Which case applies is recorded in
// f_alloc_ctx.)
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

// ------------------------------------------------ leaf mask
// Per-consumer selection of which leaves get installed. Default OP_ALL
// preserves the previous behavior; consumers that only call a subset
// (e.g. vector uses destroy + move_construct only) pass a narrower mask
// so that taking `&copy_assign_op<T>` (etc.) never happens — the body of
// an unused leaf is not instantiated, sidestepping the "implicit op= is
// declared but body-ill-formed" deep-instantiation chain (the tzdb case).
inline constexpr unsigned OP_DEFAULT_CONSTRUCT = 1u << 0;
inline constexpr unsigned OP_DESTROY           = 1u << 1;
inline constexpr unsigned OP_COPY_CONSTRUCT    = 1u << 2;
inline constexpr unsigned OP_MOVE_CONSTRUCT    = 1u << 3;
inline constexpr unsigned OP_COPY_ASSIGN       = 1u << 4;
inline constexpr unsigned OP_MOVE_ASSIGN       = 1u << 5;
inline constexpr unsigned OP_SWAP              = 1u << 6;
inline constexpr unsigned OP_COMPARE           = 1u << 7;
inline constexpr unsigned OP_EQUAL             = 1u << 8;
inline constexpr unsigned OP_HASH              = 1u << 9;
inline constexpr unsigned OP_ALL               = ~0u;

template<class T, class A, unsigned MASK = OP_ALL>
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
    // Load-bearing: the consumer needs both these bits to take the trivial
    // fast path (memcpy/memset/skip). They are independent of MASK and of the
    // leaf install decisions below.
    if constexpr (!alloc_stateless<A>)                     o.flags |= f_alloc_ctx;
    if constexpr (default_life)                            o.flags |= f_alloc_default_life;

    // Lifecycle leaves. Each leaf install is doubly gated:
    //   1. MASK bit set (consumer asked for this leaf), AND
    //   2. body-level `requires` on the actual expression the leaf will form,
    //      so the gate matches what the leaf body does — see comment block
    //      at the value-op gates below for the rationale.
    if constexpr ((MASK & OP_DEFAULT_CONSTRUCT) && requires(T* p) { ::new (p) T(); })
        if constexpr (!(is_trivially_default_constructible_v<T> && default_life))
            o.default_construct = &default_construct_op<T, A>;
    if constexpr ((MASK & OP_DESTROY) && !(is_trivially_destructible_v<T> && default_life))
        o.destroy = &destroy_op<T, A>;
    if constexpr ((MASK & OP_COPY_CONSTRUCT) && requires(T* p, const T& s) { ::new (p) T(s); })
        if constexpr (!(is_trivially_copyable_v<T> && default_life))
            o.copy_construct = &copy_construct_op<T, A>;
    if constexpr ((MASK & OP_MOVE_CONSTRUCT) && requires(T* p, T&& s) { ::new (p) T(static_cast<T&&>(s)); })
        if constexpr (!(is_trivially_relocatable_v<T> && default_life))
            o.move_construct = &move_construct_op<T, A>;

    // Value ops: not allocator-mediated; left null when the op is trivial, or
    // when it is ill-formed for T. We use `requires` over the actual
    // expression (not is_*_assignable_v) so that a type whose operator= is
    // DECLARED but ill-formed in its body (e.g. vector<MoveOnly> with an
    // unconstrained copy-assign) does not eagerly instantiate the body when
    // this table is built — only types whose copy_assign would compile reach
    // the leaf assignment expression in copy_assign_op below.
    if constexpr ((MASK & OP_COPY_ASSIGN) && !is_trivially_copyable_v<T>
                  && requires(T& d, const T& s) { d = s; })
        o.copy_assign = &copy_assign_op<T>;
    if constexpr ((MASK & OP_MOVE_ASSIGN) && !is_trivially_copyable_v<T>
                  && requires(T& d, T&& s) { d = std::move(s); })
        o.move_assign = &move_assign_op<T>;
    if constexpr ((MASK & OP_SWAP) && !is_trivially_copyable_v<T> && is_swappable_v<T>)
        o.swap = &swap_op<T>;
    if constexpr ((MASK & OP_COMPARE) && requires(const T& a, const T& b) { a < b; })
        o.compare = &compare_op<T>;
    if constexpr ((MASK & OP_EQUAL) && requires(const T& a, const T& b) { a == b; })
        o.equal = &equal_op<T>;
    if constexpr ((MASK & OP_HASH) && requires(const T& a) { std::hash<T>{}(a); })
        o.hash = &hash_op<T>;

    return o;
}

template<class T, class A, unsigned MASK = OP_ALL>
inline constexpr type_ops ops_for = make_type_ops<T, A, MASK>();

// =====================================================================
// Two-type table: ops between a T and a U
// =====================================================================
// Embeds pointers to the single-type tables for T (first) and U (second), so a
// two-type table is one pointer rather than three. The cross construct/assign
// leaves use placement-new / direct assignment; they are not allocator-mediated.
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
    // Body-level requires (same reasoning as make_type_ops): the trait
    // is_*_v says yes whenever the signature exists, but our leaf forms the
    // expression and must body-instantiate it. Using the same expression in
    // the gate prevents taking the leaf address for a T/U whose construction
    // signature is declared-but-body-ill-formed.
    if constexpr (requires(T* d, const U& s) { ::new (d) T(s); })
        o.construct_from = &construct_from_op<T, U>;
    if constexpr (requires(T& d, const U& s) { d = s; })
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
// A realloc-style function type for a container's raw storage, separate from the
// element ops. Bound to a container through one opaque `void* ctx`, so a
// non-template core takes a `realloc_op` + that ctx without naming the container
// or allocator type. It resizes a buffer and preserves its bytes; it is given no
// `type_ops` and does not touch elements.
//
//   realloc_op(ctx, cur, &size) -> new_base
//
//   ctx     the owning container (its `this`); through it the function reaches
//           the allocator and any inline/small buffer.
//   cur     current buffer base, or null if there is none.
//   size    in/out, in BYTES: on entry the requested size; on return the actual
//           size of the buffer (>= requested).
//   returns the buffer base, with the first min(old, new) bytes preserved. It
//           equals `cur` when the buffer was resized in place; when it differs,
//           any prior buffer at `cur` has been freed. Allocation failure does not
//           return null — it traps (no exceptions).
using realloc_op = void* (*)(void* ctx, void* cur, size_t* size);

} // namespace detail
} // namespace std
