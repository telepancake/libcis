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
//     COMPILE TIME by the typed wrapper, not at runtime by the core. A wrapper
//     method that needs an op is `requires`-constrained on it (exactly as
//     std::vector's push_back(const T&) is ill-formed for a move-only type), so
//     the core is only ever instantiated into a path when that op is valid.
//     Within the core, then, a null pointer unambiguously means "trivial — handle
//     inline by size" (memcpy / memmove / skip); the "invalid" case never reaches
//     the core. No runtime flag is needed to tell them apart.
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
struct cross_ops {
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
    cross_ops o{nullptr, nullptr, nullptr, nullptr, nullptr};
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

} // namespace ev
