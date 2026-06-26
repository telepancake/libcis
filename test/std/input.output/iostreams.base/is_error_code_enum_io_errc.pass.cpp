// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_is_error_code_enum_io_errc_ee87bc17).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <> struct is_error_code_enum<io_errc> : public true_type {};

#include <ios>
#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_is_error_code_enum_io_errc_ee87bc17 { // libcis
int main(int, char**)
{
    static_assert(std::is_error_code_enum  <std::io_errc>::value, "");
#if TEST_STD_VER > 14
    static_assert(std::is_error_code_enum_v<std::io_errc>, "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_is_error_code_enum_io_errc_ee87bc17 (libcis)

