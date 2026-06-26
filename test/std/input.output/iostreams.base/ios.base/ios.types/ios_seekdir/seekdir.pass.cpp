// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_types_ios_seekdir_seekdir_cc59ec11).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// static const seekdir beg;
// static const seekdir cur;
// static const seekdir end;

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_ios_types_ios_seekdir_seekdir_cc59ec11 { // libcis
int main(int, char**)
{
    assert(std::ios_base::beg != std::ios_base::cur);
    assert(std::ios_base::beg != std::ios_base::end);
    assert(std::ios_base::cur != std::ios_base::end);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_ios_types_ios_seekdir_seekdir_cc59ec11 (libcis)

