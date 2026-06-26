// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_fpos_fpos_operations_streamsize_923bd187).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// streamsize and streamoff interconvert

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_fpos_fpos_operations_streamsize_923bd187 { // libcis
int main(int, char**)
{
    std::streamoff o(5);
    std::streamsize sz(o);
    assert(sz == 5);
    std::streamoff o2(sz);
    assert(o == o2);

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_fpos_fpos_operations_streamsize_923bd187 (libcis)

