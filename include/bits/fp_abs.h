// libcis: the std::abs overload set shared by <cmath> and <cstdlib>.
// [c.math.abs]: abs is declared in BOTH headers.  The long/long long and
// floating overloads (C has fabs, not abs, for those) are provided in the
// GLOBAL namespace by <stdlib.h> (matching libc++'s include/stdlib.h, and
// what the depr.c.headers/stdlib_h test requires); this header just imports
// them into namespace std.  Keeping a SINGLE definition (global) avoids the
// redeclaration conflict that arises when both <stdlib.h> and a separate
// std-scope definition are pulled together by `using ::abs`.
#pragma once

#include <stdlib.h>   // global ::abs(long/long long/float/double/long double)

namespace std {
using ::abs;
} // namespace std
