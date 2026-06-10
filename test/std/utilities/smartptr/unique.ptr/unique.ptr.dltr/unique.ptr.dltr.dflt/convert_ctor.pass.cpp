// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_convert_ctor).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_convert_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// default_delete

// convert constructor

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_convert_ctor { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX23 bool test() {
  std::default_delete<B> d2;
  std::default_delete<A> d1 = d2;
  A* p                      = new B;
  if (!TEST_IS_CONSTANT_EVALUATED) {
    assert(A::count == 1);
    assert(B::count == 1);
  }

  d1(p);

  if (!TEST_IS_CONSTANT_EVALUATED) {
    assert(A::count == 0);
    assert(B::count == 0);
  }

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_convert_ctor; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt_convert_ctor() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
