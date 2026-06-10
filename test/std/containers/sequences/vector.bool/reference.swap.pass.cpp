// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_reference_swap).
// main -> test_containers_sequences_vector_bool_reference_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// static void swap(reference x, reference y) noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_bool_reference_swap { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool tests() {
  bool a[] = {false, true, false, true};
  bool* an = a + sizeof(a) / sizeof(a[0]);

  std::vector<bool> v(a, an);
  std::vector<bool>::reference r1 = v[0];
  std::vector<bool>::reference r2 = v[3];

#if TEST_STD_VER >= 11
  static_assert((noexcept(v.swap(r1, r2))), "");
#endif

  assert(!r1);
  assert(r2);
  v.swap(r1, r2);
  assert(r1);
  assert(!r2);

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_reference_swap; // libcis


void test_containers_sequences_vector_bool_reference_swap() {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return;
}
