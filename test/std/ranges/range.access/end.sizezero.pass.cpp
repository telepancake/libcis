// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_access_end_sizezero).
// main -> test_ranges_range_access_end_sizezero; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: msvc

// std::ranges::end
// std::ranges::cend
//   Test the fix for https://llvm.org/PR54100

#include <ranges>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_ranges_range_access_end_sizezero { // libcis: isolate file-scope helpers
struct A {
  int m[0];
};
static_assert(sizeof(A) == 0);
} using namespace libcis_ns_ranges_range_access_end_sizezero; // libcis
 // an extension supported by GCC and Clang

void test_ranges_range_access_end_sizezero()
{
  A a[10];
  std::same_as<A*> auto p = std::ranges::end(a);
  assert(p == a + 10);
  std::same_as<const A*> auto cp = std::ranges::cend(a);
  assert(cp == a + 10);

  return;
}
