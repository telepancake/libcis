// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_default).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// default_delete

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_default { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX23 bool test() {
  std::default_delete<A> d;
  A* p = new A;
  if (!TEST_IS_CONSTANT_EVALUATED)
    assert(A::count == 1);

  d(p);

  if (!TEST_IS_CONSTANT_EVALUATED)
    assert(A::count == 0);

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_default; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_default() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
