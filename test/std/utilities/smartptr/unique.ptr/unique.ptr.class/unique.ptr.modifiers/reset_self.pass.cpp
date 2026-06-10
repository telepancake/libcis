// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_self).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_self; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// unique_ptr

// test reset against resetting self

#include <memory>

#include "test_macros.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_self { // libcis: isolate file-scope helpers
struct A {
  std::unique_ptr<A> ptr_;

  TEST_CONSTEXPR_CXX23 A() : ptr_(this) {}
  TEST_CONSTEXPR_CXX23 void reset() { ptr_.reset(); }
};

TEST_CONSTEXPR_CXX23 bool test() {
  (new A)->reset();

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_self; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_self() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
