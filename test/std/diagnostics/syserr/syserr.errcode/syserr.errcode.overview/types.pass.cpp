// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcode_syserr_errcode_overview_types_82b4c3c8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>
// class error_code

// Make sure that the error_code bits of <system_error> are self-contained.

#include <system_error>
#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_overview_types_82b4c3c8 { // libcis
int main(int, char**)
{
    std::error_code x;
    TEST_IGNORE_NODISCARD  x.category();   // returns a std::error_category &
    TEST_IGNORE_NODISCARD  x.default_error_condition(); // std::error_condition
    TEST_IGNORE_NODISCARD  x.message();    // returns a std::string

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_overview_types_82b4c3c8 (libcis)

