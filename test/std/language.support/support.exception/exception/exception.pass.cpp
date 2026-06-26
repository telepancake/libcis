// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_exception_exception_exception_90cf4f31).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test exception

#include <exception>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_exception_exception_exception_90cf4f31 { // libcis
int main(int, char**)
{
    static_assert(std::is_polymorphic<std::exception>::value,
                 "std::is_polymorphic<std::exception>::value");
    std::exception b;
    std::exception b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_exception_exception_exception_90cf4f31 (libcis)

