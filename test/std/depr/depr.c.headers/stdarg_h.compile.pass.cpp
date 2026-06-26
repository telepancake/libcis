// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_c_headers_stdarg_h_d2a26ccd).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test <stdarg.h>

#include <stdarg.h>

#include "test_macros.h"

#ifndef va_arg
#error va_arg not defined
#endif

#if TEST_STD_VER >= 11
#  ifndef va_copy
#    error va_copy is not defined when c++ >= 11
#  endif
#endif

#ifndef va_end
#error va_end not defined
#endif

#ifndef va_start
#error va_start not defined
#endif

namespace libcis_ns_depr_depr_c_headers_stdarg_h_d2a26ccd { // libcis
va_list va;
} // libcis_ns_depr_depr_c_headers_stdarg_h_d2a26ccd (libcis)

