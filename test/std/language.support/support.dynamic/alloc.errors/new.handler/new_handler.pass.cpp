// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_alloc_errors_new_handler_new_handler_bcb4f227).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test new_handler

#include <new>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_alloc_errors_new_handler_new_handler_bcb4f227 { // libcis
void f() {}

int main(int, char**)
{
    static_assert((std::is_same<std::new_handler, void(*)()>::value), "");
    std::new_handler p = f;
    assert(p == &f);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_alloc_errors_new_handler_new_handler_bcb4f227 (libcis)

