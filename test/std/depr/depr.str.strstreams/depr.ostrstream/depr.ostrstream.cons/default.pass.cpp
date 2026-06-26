// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_cons_default_b336d95b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class ostrstream

// ostrstream();

#include <strstream>
#include <cassert>
#include <string>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_cons_default_b336d95b { // libcis
int main(int, char**)
{
    std::ostrstream out;
    int i = 123;
    double d = 4.5;
    std::string s("dog");
    out << i << ' ' << d << ' ' << s << std::ends;
    assert(out.str() == std::string("123 4.5 dog"));
    out.freeze(false);

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_cons_default_b336d95b (libcis)

