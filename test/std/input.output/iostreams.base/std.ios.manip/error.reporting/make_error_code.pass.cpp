// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_std_ios_manip_error_reporting_make_error_code_6fa3f390).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// error_code make_error_code(io_errc e);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_std_ios_manip_error_reporting_make_error_code_6fa3f390 { // libcis
int main(int, char**)
{
    {
        std::error_code ec = make_error_code(std::io_errc::stream);
        assert(ec.value() == static_cast<int>(std::io_errc::stream));
        assert(ec.category() == std::iostream_category());
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_std_ios_manip_error_reporting_make_error_code_6fa3f390 (libcis)

