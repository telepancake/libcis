// type_ops.hpp — universal, type-dependent leaf operations + a constexpr
// descriptor that bundles them with sizeof/alignof.
//
// This is the "make separate header of common functions" half of the
// experiment: the per-element operations a container needs (relocate on growth,
// copy-construct one, destroy a range) live here as small function templates,
// and `ops_for<T>` is a constexpr table of pointers to them. The heavy,
// type-INDEPENDENT container logic (evector.hpp) takes a `const type_ops&` and
// never gets instantiated per element type.
#pragma once
#include <cstddef>
#include <new>
#include <type_traits>
#include <utility>

namespace ev {

struct type_ops {
    std::size_t size;
    std::size_t align;
    void (*relocate)(void* dst, void* src, std::size_t n);  // move-construct dst, destroy src
    void (*copy_one)(void* dst, const void* src);           // copy-construct one element
    void (*destroy)(void* p, std::size_t n);                // destroy a range
};

template<class T>
void relocate_impl(void* dst, void* src, std::size_t n) {
    T* d = static_cast<T*>(dst);
    T* s = static_cast<T*>(src);
    for (std::size_t i = 0; i < n; ++i) {
        ::new (static_cast<void*>(d + i)) T(std::move(s[i]));
        s[i].~T();
    }
}

template<class T>
void copy_one_impl(void* dst, const void* src) {
    ::new (dst) T(*static_cast<const T*>(src));
}

template<class T>
void destroy_impl(void* p, std::size_t n) {
    T* x = static_cast<T*>(p);
    for (std::size_t i = 0; i < n; ++i) x[i].~T();
}

// Trivial types get NULL op pointers: the core handles them inline (memmove /
// memcpy / nothing) using the runtime size. Non-trivial ops are set ONLY when the
// operation is actually valid for T — so a single type_ops shape serves every
// type, and we never instantiate a leaf that would be ill-formed (e.g.
// copy_one_impl<T> for a move-only T with a deleted copy ctor). `if constexpr`
// guards both elision (don't name &impl<T> for trivial T) and validity.
template<class T>
constexpr type_ops make_ops() {
    type_ops o{sizeof(T), alignof(T), nullptr, nullptr, nullptr};
    if constexpr (!std::is_trivially_copyable_v<T>) {
        // relocate = move-construct + destroy source; valid iff T is (move-or-copy)
        // constructible (is_move_constructible_v is true when a copy ctor serves).
        if constexpr (std::is_move_constructible_v<T>) o.relocate = &relocate_impl<T>;
        if constexpr (std::is_copy_constructible_v<T>) o.copy_one = &copy_one_impl<T>;
    }
    if constexpr (!std::is_trivially_destructible_v<T>) o.destroy = &destroy_impl<T>;
    return o;
}

template<class T>
inline constexpr type_ops ops_for = make_ops<T>();

} // namespace ev
