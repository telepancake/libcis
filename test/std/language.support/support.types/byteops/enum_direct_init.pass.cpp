// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_types_byteops_enum_direct_init_b057afb9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <test_macros.h>

// UNSUPPORTED: c++03, c++11, c++14

namespace libcis_ns_language_support_support_types_byteops_enum_direct_init_b057afb9 { // libcis
int main(int, char**) {
  constexpr std::byte b{42};
  static_assert(std::to_integer<int>(b) == 42, "");

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_types_byteops_enum_direct_init_b057afb9 (libcis)

