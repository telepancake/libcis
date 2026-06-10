// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// unique_ptr

// test reset

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX23 bool test() {
  {
    std::unique_ptr<A> p(new A);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      assert(A::count == 1);
      assert(B::count == 0);
    }
    A* i = p.get();
    assert(i != nullptr);
    p.reset(new B);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      assert(A::count == 1);
      assert(B::count == 1);
    }
  }
  if (!TEST_IS_CONSTANT_EVALUATED) {
    assert(A::count == 0);
    assert(B::count == 0);
  }
  {
    std::unique_ptr<A> p(new B);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      assert(A::count == 1);
      assert(B::count == 1);
    }
    A* i = p.get();
    assert(i != nullptr);
    p.reset(new B);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      assert(A::count == 1);
      assert(B::count == 1);
    }
  }
  if (!TEST_IS_CONSTANT_EVALUATED) {
    assert(A::count == 0);
    assert(B::count == 0);
  }

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
