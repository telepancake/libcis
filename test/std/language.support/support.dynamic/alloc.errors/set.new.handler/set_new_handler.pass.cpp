// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler).
// main -> test_language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test set_new_handler

#include <new>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler { // libcis: isolate file-scope helpers
void f1() {}
void f2() {}
} using namespace libcis_ns_language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler; // libcis


void test_language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler()
{
    assert(std::set_new_handler(f1) == 0);
    assert(std::set_new_handler(f2) == f1);

  return;
}
