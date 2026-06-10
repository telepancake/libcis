// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_overview_types).
// main -> test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_overview_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>
// class error_condition

// Make sure that the error_condition bits of <system_error> are self-contained.

#include <system_error>
#include "test_macros.h"

void test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_overview_types()
{
    std::error_condition x = std::errc(0);
    TEST_IGNORE_NODISCARD  x.category();   // returns a std::error_condition &
    TEST_IGNORE_NODISCARD  x.message();    // returns a std::string

  return;
}
