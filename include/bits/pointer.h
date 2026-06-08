// bits/pointer.h — addressof, pointer_traits, to_address
// Internal header: shared by <iterator>, <memory>, and <functional>.
// Only depends on <type_traits> and <cstddef>; must not include any user header
// that would create a cycle.
//
// Mechanically ported from:
//   libc++ include/__memory/addressof.h
//   libc++ include/__memory/pointer_traits.h
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#pragma once

#include <type_traits>
#include <cstddef>

namespace std {

//===----------------------------------------------------------------------===//
// addressof
// (from libc++ __memory/addressof.h)
//===----------------------------------------------------------------------===//

template<class T>
[[nodiscard]] constexpr T* addressof(T& x) noexcept {
    return __builtin_addressof(x);
}

template<class T>
const T* addressof(const T&&) = delete;

//===----------------------------------------------------------------------===//
// pointer_traits
// (from libc++ __memory/pointer_traits.h)
//===----------------------------------------------------------------------===//

// --- element_type detection ---

// Primary: try to extract first template argument of Ptr<T, ...>
namespace detail {
    template<class Ptr>
    struct pointer_traits_element_type_impl {};

    template<template<class, class...> class Sp, class T, class... Args>
    struct pointer_traits_element_type_impl<Sp<T, Args...>> {
        using type = T;
    };

    template<class Ptr, class = void>
    struct pointer_traits_element_type
        : pointer_traits_element_type_impl<Ptr> {};

    template<class Ptr>
    struct pointer_traits_element_type<Ptr, void_t<typename Ptr::element_type>> {
        using type = typename Ptr::element_type;
    };
} // namespace detail

// --- rebind detection ---

namespace detail {
    // Fallback: rebind by substituting the first template argument
    template<class Ptr, class U>
    struct pointer_traits_rebind_impl {
        static_assert(sizeof(Ptr) == 0,
            "Cannot rebind pointer; did you forget to add a rebind member to your pointer?");
    };

    template<template<class, class...> class Sp, class T, class... Args, class U>
    struct pointer_traits_rebind_impl<Sp<T, Args...>, U> {
        using type = Sp<U, Args...>;
    };

    template<class Ptr, class U, class = void>
    struct pointer_traits_rebind : pointer_traits_rebind_impl<Ptr, U> {};

    template<class Ptr, class U>
    struct pointer_traits_rebind<Ptr, U, void_t<typename Ptr::template rebind<U>>> {
        using type = typename Ptr::template rebind<U>;
    };
} // namespace detail

// --- pointer_traits primary template ---

namespace detail {
    template<class Ptr, class = void>
    struct pointer_traits_impl {};

    template<class Ptr>
    struct pointer_traits_impl<Ptr,
        void_t<typename pointer_traits_element_type<Ptr>::type>>
    {
        using pointer       = Ptr;
        using element_type  = typename pointer_traits_element_type<Ptr>::type;
        using difference_type =
            conditional_t<requires { typename Ptr::difference_type; },
                          typename Ptr::difference_type,
                          ptrdiff_t>;

        template<class U>
        using rebind = typename pointer_traits_rebind<pointer, U>::type;

        static constexpr pointer
        pointer_to(conditional_t<is_void_v<element_type>, int, element_type>& r) {
            return pointer::pointer_to(r);
        }
    };
} // namespace detail

template<class Ptr>
struct pointer_traits : detail::pointer_traits_impl<Ptr> {};

// --- pointer_traits<T*> specialisation ---

template<class T>
struct pointer_traits<T*> {
    using pointer         = T*;
    using element_type    = T;
    using difference_type = ptrdiff_t;

    template<class U>
    using rebind = U*;

    static constexpr pointer
    pointer_to(conditional_t<is_void_v<element_type>, int, element_type>& r) noexcept {
        return addressof(r);
    }
};

//===----------------------------------------------------------------------===//
// to_address
// (from libc++ __memory/pointer_traits.h, C++20 path)
//===----------------------------------------------------------------------===//

template<class T>
constexpr T* to_address(T* p) noexcept {
    static_assert(!is_function_v<T>, "to_address: argument must not be a function pointer");
    return p;
}

// Helper: does pointer_traits<Ptr>::to_address(p) compile?
namespace detail {
    template<class Ptr, class = void>
    inline constexpr bool has_pointer_traits_to_address = false;

    template<class Ptr>
    inline constexpr bool has_pointer_traits_to_address<Ptr,
        decltype((void)pointer_traits<Ptr>::to_address(declval<const Ptr&>()))> = true;
} // namespace detail

template<class Ptr>
constexpr auto to_address(const Ptr& p) noexcept
    -> decltype(to_address(p.operator->()))
{
    if constexpr (detail::has_pointer_traits_to_address<Ptr>) {
        return pointer_traits<Ptr>::to_address(p);
    } else {
        return to_address(p.operator->());
    }
}

} // namespace std
