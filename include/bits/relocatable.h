// bits/relocatable.h — trivial-relocatability trait for libcis.
//
// A type is *trivially relocatable* if moving it to fresh storage and ending the
// source object's lifetime is equivalent to copying its bytes (memcpy) and
// forgetting the source — no move ctor / dtor needed.  This is WEAKER than
// is_trivially_copyable (which also needs trivial copy/assign): std::string,
// std::vector, std::unique_ptr are trivially relocatable but NOT trivially
// copyable.  C++20 has no such trait (P1144 is not in, gcc-10 has no builtin),
// so this is a hand-maintained trait: trivially-copyable types qualify by
// default, and library types opt in by specialization.
//
// libcis's basic_string uses the libc++ SSO representation (inline buffer reached
// via `this`, NOT a stored self-pointer), so it is trivially relocatable.
#pragma once
#include <type_traits>

namespace std {

template<class CharT, class Traits, class Allocator> class basic_string;
template<class T, class Allocator> class vector;
template<class T, class Deleter> class unique_ptr;

namespace detail {

template<class T>
inline constexpr bool is_trivially_relocatable_v = is_trivially_copyable_v<T>;

template<class C, class Tr, class A>
inline constexpr bool is_trivially_relocatable_v<basic_string<C, Tr, A>> = true;

template<class T, class A>
inline constexpr bool is_trivially_relocatable_v<vector<T, A>> = true;

template<class T, class D>
inline constexpr bool is_trivially_relocatable_v<unique_ptr<T, D>> = true;

} // namespace detail
} // namespace std
