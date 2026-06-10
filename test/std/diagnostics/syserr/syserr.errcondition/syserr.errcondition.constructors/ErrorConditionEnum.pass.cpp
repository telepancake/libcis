// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_constructors_ErrorConditionEnum).
// main -> test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_constructors_ErrorConditionEnum; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// template <ErrorConditionEnum E> error_condition(E e);

#include <system_error>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_constructors_ErrorConditionEnum()
{
    {
        std::error_condition ec(std::errc::not_a_directory);
        assert(ec.value() == static_cast<int>(std::errc::not_a_directory));
        assert(ec.category() == std::generic_category());
    }

  return;
}
