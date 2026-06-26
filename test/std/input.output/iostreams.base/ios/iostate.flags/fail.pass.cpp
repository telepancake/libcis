// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_fail_40e66807).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// bool fail() const;

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_iostate_flags_fail_40e66807 { // libcis
struct testbuf : public std::streambuf {};

int main(int, char**)
{
    {
        std::ios ios(0);
        assert(ios.fail());
        ios.setstate(std::ios::eofbit);
        assert(ios.fail());
    }
    {
        testbuf sb;
        std::ios ios(&sb);
        assert(!ios.fail());
        ios.setstate(std::ios::eofbit);
        assert(!ios.fail());
        ios.setstate(std::ios::badbit);
        assert(ios.fail());
        ios.setstate(std::ios::failbit);
        assert(ios.fail());
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_iostate_flags_fail_40e66807 (libcis)

