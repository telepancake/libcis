// libcis: the std::abs overload set shared by <cmath> and <cstdlib>.
// [c.math.abs]: abs is declared in BOTH headers, so the definitions live here
// once.  glibc's stdlib.h declares only the C int abs(int); the long/long long
// overloads (and the floating ones -- C has fabs, not abs, for those) are the
// C++ library's to provide.  Without the floating overloads std::abs(1.5)
// binds abs(int) and truncates.  (Same layout as libstdc++ bits/std_abs.h.)
#pragma once

namespace std {

inline long        abs(long x)        { return __builtin_labs(x); }
inline long long   abs(long long x)   { return __builtin_llabs(x); }
inline float       abs(float x)       { return __builtin_fabsf(x); }
inline double      abs(double x)      { return __builtin_fabs(x); }
inline long double abs(long double x) { return __builtin_fabsl(x); }

} // namespace std
