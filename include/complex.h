// <complex.h> — ported from libc++ include/complex.h.
// In C++ this header is simply <complex>; it deliberately does NOT pull in the
// C <complex.h> (which would define the `complex` macro and clash with
// std::complex).  libcis is C++-only, so the C include_next branch is dropped.
#pragma once

#include <complex>
