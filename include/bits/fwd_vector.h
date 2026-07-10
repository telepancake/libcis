// bits/fwd_vector.h — forward declaration of std::vector.
//
// Mirrors libc++'s <__fwd/vector.h>: a single, cheap forward declaration of
// vector (with its default allocator argument) that widely-included headers can
// pull in without dragging the whole <vector> definition. This is the ONE place
// the default template argument is spelled; <vector> includes this header and
// its own declaration/definition therefore must NOT repeat the default.
//
// Test support headers such as deduction_guides_sfinae_checks.h name
// std::vector<T> at parse time while only including <memory>/<functional>/etc.,
// so those headers include this to make the name visible (libc++ makes it
// visible the same way).
#pragma once

namespace std {

template<class T>
class allocator;

template<class T, class Allocator = allocator<T>>
class vector;

} // namespace std
