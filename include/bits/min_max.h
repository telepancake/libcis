// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Two-argument std::min / std::max (operator< and comparator forms).
//
// Factored out of <algorithm> (libc++ __algorithm/min.h, max.h) so that
// headers which cannot include the whole of <algorithm> (e.g. <ranges>, which
// <algorithm> itself includes) can still use std::min / std::max without
// duplicating the definition. <algorithm> includes this header for the
// initializer_list overloads and the rest of <algorithm>.
//===----------------------------------------------------------------------===//

#ifndef _LIBCIS_BITS_MIN_MAX_H
#define _LIBCIS_BITS_MIN_MAX_H

namespace std {

// min (comparator)
template<class T, class Compare>
[[nodiscard]] constexpr const T&
min(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? b : a;
}

// min (operator<)
template<class T>
[[nodiscard]] constexpr const T&
min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

// max (comparator)
template<class T, class Compare>
[[nodiscard]] constexpr const T&
max(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}

// max (operator<)
template<class T>
[[nodiscard]] constexpr const T&
max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

} // namespace std

#endif // _LIBCIS_BITS_MIN_MAX_H
