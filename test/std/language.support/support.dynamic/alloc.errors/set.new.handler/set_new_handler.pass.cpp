// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler_b0dd15cc).
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

namespace libcis_ns_language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler_b0dd15cc { // libcis
void f1() {}
void f2() {}

int main(int, char**)
{
    assert(std::set_new_handler(f1) == 0);
    assert(std::set_new_handler(f2) == f1);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_alloc_errors_set_new_handler_set_new_handler_b0dd15cc (libcis)

