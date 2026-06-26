// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_strstream_depr_strstream_dest_rdbuf_039f47f5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class strstream

// strstreambuf* rdbuf() const;

#include <strstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_strstream_depr_strstream_dest_rdbuf_039f47f5 { // libcis
int main(int, char**)
{
    {
        char buf[] = "123 4.5 dog";
        const std::strstream out(buf, 0);
        std::strstreambuf* sb = out.rdbuf();
        assert(sb->sputc('a') == 'a');
        assert(buf == std::string("a23 4.5 dog"));
    }

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_strstream_depr_strstream_dest_rdbuf_039f47f5 (libcis)

