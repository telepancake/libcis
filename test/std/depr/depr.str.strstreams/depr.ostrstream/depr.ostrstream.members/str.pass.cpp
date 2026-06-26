// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_members_str_30af9110).
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

// char* str();

#include <strstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_members_str_30af9110 { // libcis
int main(int, char**)
{
    {
        std::ostrstream out;
        out << 123 << ' ' << 4.5 << ' ' << "dog" << std::ends;
        assert(out.str() == std::string("123 4.5 dog"));
        out.freeze(false);
    }

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_members_str_30af9110 (libcis)

