// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcat_syserr_errcat_virtuals_default_error_condition).
// main -> test_diagnostics_syserr_syserr_errcat_syserr_errcat_virtuals_default_error_condition; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_category

// virtual error_condition default_error_condition(int ev) const;

#include <system_error>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_syserr_syserr_errcat_syserr_errcat_virtuals_default_error_condition()
{
    const std::error_category& e_cat = std::generic_category();
    std::error_condition e_cond = e_cat.default_error_condition(static_cast<int>(std::errc::not_a_directory));
    assert(e_cond.category() == e_cat);
    assert(e_cond.value() == static_cast<int>(std::errc::not_a_directory));

  return;
}
