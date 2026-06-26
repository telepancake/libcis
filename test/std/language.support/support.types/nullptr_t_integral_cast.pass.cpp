// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_types_nullptr_t_integral_cast_a472e7aa).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// NOTE: nullptr_t emulation (used in libc++ ABI v.1) cannot handle a
// reinterpret_cast to an integral type
// UNSUPPORTED: c++03

// typedef decltype(nullptr) nullptr_t;


#include <cstddef>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_types_nullptr_t_integral_cast_a472e7aa { // libcis
int main(int, char**)
{
    std::ptrdiff_t i = reinterpret_cast<std::ptrdiff_t>(nullptr);
    assert(i == 0);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_types_nullptr_t_integral_cast_a472e7aa (libcis)

