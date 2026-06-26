// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_types_size_t_d8efc491).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <type_traits>

#include "test_macros.h"

// size_t should:

//  1. be in namespace std.
//  2. be the same sizeof as void*.
//  3. be an unsigned integral.

namespace libcis_ns_language_support_support_types_size_t_d8efc491 { // libcis
int main(int, char**)
{
    static_assert(sizeof(std::size_t) == sizeof(void*),
                  "sizeof(std::size_t) == sizeof(void*)");
    static_assert(std::is_unsigned<std::size_t>::value,
                  "std::is_unsigned<std::size_t>::value");
    static_assert(std::is_integral<std::size_t>::value,
                  "std::is_integral<std::size_t>::value");

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_types_size_t_d8efc491 (libcis)

