// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_stream_types_streamoff_c82b9319).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// typedef OFF_T streamoff;

#include <ios>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_stream_types_streamoff_c82b9319 { // libcis
int main(int, char**)
{
    static_assert(std::is_integral<std::streamoff>::value, "");
    static_assert(std::is_signed<std::streamoff>::value, "");

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_stream_types_streamoff_c82b9319 (libcis)

