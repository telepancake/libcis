// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_exception_exception_terminate_set_terminate_set_terminate).
// main -> test_language_support_support_exception_exception_terminate_set_terminate_set_terminate; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test set_terminate

#include <exception>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_exception_exception_terminate_set_terminate_set_terminate { // libcis: isolate file-scope helpers
void f1() {}
void f2() {}
} using namespace libcis_ns_language_support_support_exception_exception_terminate_set_terminate_set_terminate; // libcis


void test_language_support_support_exception_exception_terminate_set_terminate_set_terminate()
{
    std::set_terminate(f1);
    assert(std::set_terminate(f2) == f1);

  return;
}
