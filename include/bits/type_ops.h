// bits/type_ops.h — the type-erasure operation table and the const type_ops*
// ABI, in the library (not a bench experiment this time).
//
// A type_ops describes one element type to the shared, non-template container
// cores: the element size plus pointers to the type-dependent leaf operations.
// A null leaf is the trivial sentinel (move_construct == null => trivially
// relocatable, the core memcpy's; destroy == null => trivially destructible,
// nothing to do). Each pointer is set only when the op is valid AND non-trivial
// (if constexpr guards), so a single type_ops shape serves every element type and
// no ill-formed leaf is instantiated.
//
// The cores take a single `const type_ops&` (one pointer), shared across all
// element types and reused by any container/algorithm that relocates elements.
#pragma once
#include <cstddef>
#include <new>
#include <type_traits>

namespace std {
namespace detail {

struct type_ops {
    size_t size;
    void (*move_construct)(void*, void*);        // null: trivially relocatable
    void (*copy_construct)(void*, const void*);  // null: not copyable (move-only)
    void (*destroy)(void*);                      // null: trivially destructible
};

template<class T> void move_construct_op(void* d, void* s) {
    ::new (d) T(static_cast<T&&>(*static_cast<T*>(s)));
}
template<class T> void copy_construct_op(void* d, const void* s) {
    ::new (d) T(*static_cast<const T*>(s));
}
template<class T> void destroy_op(void* p) { static_cast<T*>(p)->~T(); }

template<class T>
constexpr type_ops make_type_ops() {
    type_ops o{sizeof(T), nullptr, nullptr, nullptr};
    if constexpr (!is_trivially_copyable_v<T>) {
        if constexpr (is_move_constructible_v<T>) o.move_construct = &move_construct_op<T>;
        if constexpr (is_copy_constructible_v<T>) o.copy_construct = &copy_construct_op<T>;
    }
    if constexpr (!is_trivially_destructible_v<T>) o.destroy = &destroy_op<T>;
    return o;
}

template<class T>
inline constexpr type_ops ops_for = make_type_ops<T>();

// THE const type_ops* core (defined once in src/support.cpp): relocate n elements
// from src to fresh dst — memcpy when trivially relocatable, else move-construct
// each then destroy the source. One shared body for every element type.
void core_relocate(void* dst, void* src, size_t n, const type_ops& ops);

} // namespace detail
} // namespace std
