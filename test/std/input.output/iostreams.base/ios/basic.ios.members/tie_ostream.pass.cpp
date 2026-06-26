// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_members_tie_ostream_14a7bd9f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// basic_ostream<charT,traits>* tie(basic_ostream<charT,traits>* tiestr);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_basic_ios_members_tie_ostream_14a7bd9f { // libcis
int main(int, char**)
{
    std::ios ios(0);
    std::ostream* os = (std::ostream*)1;
    std::ostream* r = ios.tie(os);
    assert(r == 0);
    assert(ios.tie() == os);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_basic_ios_members_tie_ostream_14a7bd9f (libcis)

