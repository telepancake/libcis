// <stdlib.h> — C general utilities, with the C++ global-namespace overloads.
// Ported from libc++ include/stdlib.h: pull in the underlying C <stdlib.h>,
// then add the floating-point std::abs overloads and the long/long long
// abs/div overloads to the GLOBAL namespace (the C library only provides the
// int forms, but C++ programs expect unqualified abs/div to work on these
// types — [c.math.abs]).
#pragma once

// glibc declarations must keep default visibility (see the cstddef note).
#pragma GCC visibility push(default)
#include_next <stdlib.h>
#pragma GCC visibility pop

#ifdef __cplusplus
extern "C++" {

#ifdef abs
#  undef abs
#endif
#ifdef labs
#  undef labs
#endif
#ifdef llabs
#  undef llabs
#endif

[[nodiscard]] inline long      abs(long __x) noexcept      { return __builtin_labs(__x); }
[[nodiscard]] inline long long abs(long long __x) noexcept { return __builtin_llabs(__x); }
[[nodiscard]] inline float     abs(float __x) noexcept       { return __builtin_fabsf(__x); }
[[nodiscard]] inline double    abs(double __x) noexcept      { return __builtin_fabs(__x); }
[[nodiscard]] inline long double abs(long double __x) noexcept { return __builtin_fabsl(__x); }

#ifdef div
#  undef div
#endif
#ifdef ldiv
#  undef ldiv
#endif
#ifdef lldiv
#  undef lldiv
#endif

inline ldiv_t  div(long __x, long __y) noexcept            { return ::ldiv(__x, __y); }
inline lldiv_t div(long long __x, long long __y) noexcept  { return ::lldiv(__x, __y); }

} // extern "C++"
#endif // __cplusplus
