// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single_24dd2c86).
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

TEST_CONSTEXPR_CXX23 namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single_24dd2c86 { // libcis
bool test() {
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

int main(int, char**) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;

    return 0;
}
} // libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single_24dd2c86 (libcis)

