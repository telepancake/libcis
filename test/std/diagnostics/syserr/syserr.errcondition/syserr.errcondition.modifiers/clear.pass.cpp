// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_clear).
// main -> test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_clear; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// void clear();

#include <system_error>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_clear()
{
    {
        std::error_condition ec;
        ec.assign(6, std::system_category());
        assert(ec.value() == 6);
        assert(ec.category() == std::system_category());
        ec.clear();
        assert(ec.value() == 0);
        assert(ec.category() == std::generic_category());
    }

  return;
}
