// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_stdcvt_codecvt_mode_eae3453e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <codecvt>

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_CODECVT

// enum codecvt_mode
// {
//     consume_header = 4,
//     generate_header = 2,
//     little_endian = 1
// };

#include <codecvt>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_stdcvt_codecvt_mode_eae3453e { // libcis
int main(int, char**)
{
    assert(std::consume_header == 4);
    assert(std::generate_header == 2);
    assert(std::little_endian == 1);
    std::codecvt_mode e = std::consume_header;
    assert(e == 4);

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_stdcvt_codecvt_mode_eae3453e (libcis)

