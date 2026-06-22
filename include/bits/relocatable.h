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
#include <new>             // placement new
#include <utility>         // std::move

namespace std {

template<class CharT, class Traits, class Allocator> class basic_string;
template<class T, class Allocator> class vector;
template<class T, class Deleter> class unique_ptr;

namespace detail {

// =========================================================================
// Shared erased-storage leaves (the unified vocabulary)
// =========================================================================
// The non-allocator-threaded type-erasure vocabularies (value_ops for <any>,
// sum_ops for <variant>, callable_ops for <functional>) all need the SAME
// three primitive operations on an object that lives at a void* address:
//
//   erased_destroy_op<T>         (void* p)                  p->~T()
//   erased_copy_construct_op<T>  (void* d, const void* s)   ::new(d) T(*s)
//   erased_move_construct_op<T>  (void* d, void* s)         ::new(d) T(move(*s))
//
// These signatures and bodies are byte-identical across the three headers, but
// when each header declared its OWN template (alt_destroy_op, destroy_small_op,
// func_destroy_op, ...) the compiler emitted a SEPARATE weak symbol per header
// per T, and ICF did not fold them (their addresses are taken into ops tables).
// A type T routed through both <any> and <variant> therefore paid for two
// copies of destroy and two of copy_construct. Sharing one template here makes
// all the vocabularies name the SAME symbol per T, so it is emitted once and
// every header's ops table points at it. (move semantics: this is the
// "construct dst from move(*src), leave src alive for the caller to destroy"
// shape — variant's and function's move-construct. <any>'s small-buffer move,
// which also destroys the source, keeps its own leaf below.)
template<class T> void erased_destroy_op(void* p) {
    static_cast<T*>(p)->~T();
}
template<class T> void erased_copy_construct_op(void* d, const void* s) {
    ::new (d) T(*static_cast<const T*>(s));
}
template<class T> void erased_move_construct_op(void* d, void* s) {
    ::new (d) T(static_cast<T&&>(*static_cast<T*>(s)));
}

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
