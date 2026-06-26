// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_std_ios_manip_error_reporting_make_error_condition_65beb750).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// error_condition make_error_condition(io_errc e);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_std_ios_manip_error_reporting_make_error_condition_65beb750 { // libcis
int main(int, char**)
{
    {
        const std::error_condition ec1 = std::make_error_condition(std::io_errc::stream);
        assert(ec1.value() == static_cast<int>(std::io_errc::stream));
        assert(ec1.category() == std::iostream_category());
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_std_ios_manip_error_reporting_make_error_condition_65beb750 (libcis)

