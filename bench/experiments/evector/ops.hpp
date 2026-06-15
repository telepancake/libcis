// ops.hpp — type-erasure operation tables.
//
// Two tables, by arity of the *types* involved (not the count of value args):
//
//   type_ops          — ops on a single element type T (lifecycle, ordering,
//                        equality, hash, swap). "Cannot meaningfully involve a
//                        second type."
//   cross_ops<T,U>     — ops genuinely between two types (construct/assign a T
//                        from a U, compare/equal a T against a U, cast U->T).
//
// Design rules, both tables:
//   * A function pointer is set IFF the operation is *valid* for the type(s) AND
//     non-trivial — guarded by `requires` / traits via `if constexpr`, so an
//     ill-formed leaf (copy of a move-only type, a cast that doesn't compile) is
//     never instantiated, and a trivial op's per-type leaf is never instantiated
//     either. So a set pointer means "non-trivial, call me"; null means "trivial
//     OR invalid".
//   * That nullptr is NOT ambiguous in practice, because VALIDITY is enforced at
//     COMPILE TIME by the typed wrapper, not at runtime by the core. The wrapper
//     restores the check the erased memcpy-fallback would otherwise swallow with a
//     `static_assert` in the method BODY (e.g. push_back(const T&) asserts
//     is_copy_constructible_v<T>) -- NOT a signature `requires`-clause. A body
//     assert fires only when the method is odr-used, leaves the candidate set /
//     trait detection / overload resolution byte-identical, and mirrors how the
//     standard itself handles CopyInsertable. (Slapping `requires` on members
//     silently redirects overload resolution -- e.g. a constrained-away member
//     `swap` falls back to std::swap/ADL with different semantics.) So the core is
//     only ever instantiated into a path when the op is valid, and within it a
//     null pointer unambiguously means "trivial -- handle inline by size"; the
//     "invalid" case never reaches the core. No runtime flag is needed.
//   * The `flags` bitset is therefore DIAGNOSTIC, not load-bearing: it records
//     triviality for introspection / static_assert / debugging. The core
//     dispatches on null-vs-set, not on flags. Zero runtime cost.
#pragma once
#include <cstddef>
#include <functional>
#include <new>
#include <type_traits>
#include <utility>

namespace ev {

// ------------------------------------------------- triviality (diagnostic only)
// Records why an op pointer is null (trivial vs not). The core does NOT read
// these — it dispatches on null-vs-set; these are for introspection / asserts /
// debugging, and cost nothing at runtime.
enum ops_flags : unsigned {
    f_triv_default = 1u << 0,  // trivially default-constructible
    f_triv_copy    = 1u << 1,  // trivially copyable (copy/move/assign == memcpy)
    f_triv_reloc   = 1u << 2,  // trivially relocatable (grow == memmove)
    f_triv_destroy = 1u << 3,  // trivially destructible (destroy == nothing)
};

// =====================================================================
// Single-type table
// =====================================================================
struct type_ops {
    std::size_t size;
    std::size_t align;
    unsigned flags;

    void   (*default_construct)(void*);              // null: trivial(flag) or invalid
    void   (*destroy)(void*);                        // null: trivial(flag)
    void   (*copy_construct)(void*, const void*);    // null: trivial(flag) or move-only
    void   (*move_construct)(void*, void*);          // null: trivial(flag) or immovable
    void   (*copy_assign)(void*, const void*);       // null: trivial(flag) or not copy-assignable
    void   (*move_assign)(void*, void*);             // null: trivial(flag) or not move-assignable
    void   (*swap)(void*, void*);                    // null: trivial(flag) or not swappable
    int    (*compare)(const void*, const void*);     // 3-way; null: not ordered
    bool   (*equal)(const void*, const void*);       // null: not equality-comparable
    std::size_t (*hash)(const void*);                // null: not hashable
};

// ---- single-type leaf ops (instantiated only when named in make_type_ops) ----
template<class T> void default_construct_op(void* p) { ::new (p) T(); }
template<class T> void destroy_op(void* p) { static_cast<T*>(p)->~T(); }
template<class T> void copy_construct_op(void* d, const void* s) {
    ::new (d) T(*static_cast<const T*>(s));
}
template<class T> void move_construct_op(void* d, void* s) {
    ::new (d) T(std::move(*static_cast<T*>(s)));
}
template<class T> void copy_assign_op(void* d, const void* s) {
    *static_cast<T*>(d) = *static_cast<const T*>(s);
}
template<class T> void move_assign_op(void* d, void* s) {
    *static_cast<T*>(d) = std::move(*static_cast<T*>(s));
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
template<class T> std::size_t hash_op(const void* p) {
    return std::hash<T>{}(*static_cast<const T*>(p));
}

template<class T>
constexpr type_ops make_type_ops() {
    type_ops o{sizeof(T), alignof(T), 0,
               nullptr, nullptr, nullptr, nullptr,
               nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    if constexpr (std::is_trivially_default_constructible_v<T>) o.flags |= f_triv_default;
    if constexpr (std::is_trivially_copyable_v<T>)              o.flags |= f_triv_copy | f_triv_reloc;
    if constexpr (std::is_trivially_destructible_v<T>)          o.flags |= f_triv_destroy;

    // lifecycle: pointer set only when valid AND non-trivial (trivial -> flag+null)
    if constexpr (std::is_default_constructible_v<T> && !std::is_trivially_default_constructible_v<T>)
        o.default_construct = &default_construct_op<T>;
    if constexpr (!std::is_trivially_destructible_v<T>)
        o.destroy = &destroy_op<T>;
    if constexpr (std::is_copy_constructible_v<T> && !std::is_trivially_copyable_v<T>)
        o.copy_construct = &copy_construct_op<T>;
    if constexpr (std::is_move_constructible_v<T> && !std::is_trivially_copyable_v<T>)
        o.move_construct = &move_construct_op<T>;
    if constexpr (std::is_copy_assignable_v<T> && !std::is_trivially_copyable_v<T>)
        o.copy_assign = &copy_assign_op<T>;
    if constexpr (std::is_move_assignable_v<T> && !std::is_trivially_copyable_v<T>)
        o.move_assign = &move_assign_op<T>;
    if constexpr (std::is_swappable_v<T> && !std::is_trivially_copyable_v<T>)
        o.swap = &swap_op<T>;

    // relational / hash: validity-gated by the actual expression (no trivial form)
    if constexpr (requires(const T& a, const T& b) { a < b; })
        o.compare = &compare_op<T>;
    if constexpr (requires(const T& a, const T& b) { a == b; })
        o.equal = &equal_op<T>;
    if constexpr (requires(const T& a) { std::hash<T>{}(a); })
        o.hash = &hash_op<T>;

    return o;
}

template<class T>
inline constexpr type_ops ops_for = make_type_ops<T>();

// =====================================================================
// Two-type table: ops between a T and a U
// =====================================================================
// Embeds pointers to the single-type tables for T (first) and U (second), so a
// two-type generic algorithm receives ONE ops pointer instead of three.
struct cross_ops {
    const type_ops* first;                       // single-type ops for T
    const type_ops* second;                      // single-type ops for U
    void (*construct_from)(void*, const void*);  // ::new(T) T(U)   null: not constructible
    void (*assign_from)(void*, const void*);     // T = U           null: not assignable
    int  (*compare)(const void*, const void*);   // T <=> U (3-way) null: not comparable
    bool (*equal)(const void*, const void*);     // T == U          null: not equality-comparable
    void (*cast)(void*, const void*);            // ::new(T) T(static_cast<T>(U))  null: cast won't compile
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

template<class T, class U>
constexpr cross_ops make_cross_ops() {
    cross_ops o{&ops_for<T>, &ops_for<U>, nullptr, nullptr, nullptr, nullptr, nullptr};
    if constexpr (std::is_constructible_v<T, const U&>)
        o.construct_from = &construct_from_op<T, U>;
    if constexpr (std::is_assignable_v<T&, const U&>)
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
// Storage management: the third axis
// =====================================================================
// insert / erase / grow must (re)allocate without knowing WHERE the storage
// lives — vector's heap buffer, string's SSO-or-heap, or a caller-supplied buffer
// in a global algorithm. That policy is erased behind a single realloc-style
// function + an opaque context pointer (the owner's `this`), typically supplied by
// the container as an inline friend bound to `this`.
//
//   realloc(ctx, cur, count, new_cap, ops, &out_cap):
//     * ensure room for >= new_cap elements in ctx's storage;
//     * preserve the first `count` elements, relocating them via `ops`
//       (ops->move_construct + destroy, or memmove when trivially relocatable);
//     * free the old storage; return the new base; write achieved cap to out_cap;
//     * may return `cur` unchanged (in-place grow / SSO / fixed storage).
//   One call performs ALL allocation work.
using realloc_fn = void* (*)(void* ctx, void* cur, std::size_t count,
                             std::size_t new_cap, const type_ops* ops,
                             std::size_t* out_cap);

// Generic-algorithm calling convention => ONE or THREE extra arguments:
//   * non-allocating (find / compare / for_each):   (const type_ops*)            1
//       two-type variant:                           (const cross_ops*)           1   (embeds both)
//   * allocating    (insert / erase / grow / assign): (ops, realloc_fn, void* ctx) 3
//   (cross_ops embeds first/second so a two-type op still passes ONE ops pointer.)

} // namespace ev
