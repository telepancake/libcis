// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_cons_cscp_size_97aa2ae9).
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

// strstreambuf(const signed char* gnext_arg, streamsize n);

#include <strstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_cons_cscp_size_97aa2ae9 { // libcis
int main(int, char**)
{
    {
        const signed char buf[] = "abcd";
        std::strstreambuf sb(buf, sizeof(buf));
        assert(sb.sgetc() == 'a');
        assert(sb.snextc() == 'b');
        assert(sb.snextc() == 'c');
        assert(sb.snextc() == 'd');
        assert(sb.snextc() == 0);
        assert(sb.snextc() == EOF);
    }
    {
        const signed char buf[] = "abcd";
        std::strstreambuf sb(buf, 0);
        assert(sb.sgetc() == 'a');
        assert(sb.snextc() == 'b');
        assert(sb.snextc() == 'c');
        assert(sb.snextc() == 'd');
        assert(sb.snextc() == EOF);
    }

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_cons_cscp_size_97aa2ae9 (libcis)

