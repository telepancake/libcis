// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_base_locales_getloc_e5cf586e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// locale getloc() const;

#include <ios>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_ios_base_locales_getloc_e5cf586e { // libcis
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};

int main(int, char**)
{
    const test t;
    assert(t.getloc().name() == std::string("C"));

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_ios_base_locales_getloc_e5cf586e (libcis)

