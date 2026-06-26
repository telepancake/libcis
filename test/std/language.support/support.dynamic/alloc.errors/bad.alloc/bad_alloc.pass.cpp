// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_alloc_errors_bad_alloc_bad_alloc_8c97d740).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test bad_alloc

#include <new>
#include <cassert>
#include <exception>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_alloc_errors_bad_alloc_bad_alloc_8c97d740 { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::exception, std::bad_alloc>::value),
                 "std::is_base_of<std::exception, std::bad_alloc>::value");
    static_assert(std::is_polymorphic<std::bad_alloc>::value,
                 "std::is_polymorphic<std::bad_alloc>::value");
    std::bad_alloc b;
    std::bad_alloc b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_alloc_errors_bad_alloc_bad_alloc_8c97d740 (libcis)

