// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_members_freeze_d8d12237).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class strstreambuf

// void freeze(bool freezefl = true);

#include <strstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_members_freeze_d8d12237 { // libcis
int main(int, char**)
{
    {
        std::strstreambuf sb;
        sb.freeze(true);
        assert(sb.sputc('a') == EOF);
        sb.freeze(false);
        assert(sb.sputc('a') == 'a');
    }

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_members_freeze_d8d12237 (libcis)

