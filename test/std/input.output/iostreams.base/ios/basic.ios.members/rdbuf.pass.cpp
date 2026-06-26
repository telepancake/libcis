// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_members_rdbuf_18e67355).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// basic_streambuf<charT,traits>* rdbuf() const;

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_basic_ios_members_rdbuf_18e67355 { // libcis
int main(int, char**)
{
    {
        const std::ios ios(0);
        assert(ios.rdbuf() == 0);
    }
    {
        std::streambuf* sb = (std::streambuf*)1;
        const std::ios ios(sb);
        assert(ios.rdbuf() == sb);
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_basic_ios_members_rdbuf_18e67355 (libcis)

