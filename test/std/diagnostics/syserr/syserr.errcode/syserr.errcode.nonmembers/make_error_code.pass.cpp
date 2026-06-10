// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcode_syserr_errcode_nonmembers_make_error_code).
// main -> test_diagnostics_syserr_syserr_errcode_syserr_errcode_nonmembers_make_error_code; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_code

// error_code make_error_code(errc e);

#include <system_error>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_syserr_syserr_errcode_syserr_errcode_nonmembers_make_error_code()
{
    {
        std::error_code ec = make_error_code(std::errc::operation_canceled);
        assert(ec.value() == static_cast<int>(std::errc::operation_canceled));
        assert(ec.category() == std::generic_category());
    }

  return;
}
