// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_members_pcount_0fbd8b5a).
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

// int pcount() const;

#include <strstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_members_pcount_0fbd8b5a { // libcis
int main(int, char**)
{
    {
        std::ostrstream out;
        assert(out.pcount() == 0);
        out << 123 << ' ' << 4.5 << ' ' << "dog";
        assert(out.pcount() == 11);
    }

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_ostrstream_depr_ostrstream_members_pcount_0fbd8b5a (libcis)

