// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_nonmembers_make_error_condition).
// main -> test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_nonmembers_make_error_condition; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// error_condition make_error_condition(errc e);

#include <system_error>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_nonmembers_make_error_condition()
{
    {
        const std::error_condition ec1 = std::make_error_condition(std::errc::message_size);
        assert(ec1.value() == static_cast<int>(std::errc::message_size));
        assert(ec1.category() == std::generic_category());
    }

  return;
}
