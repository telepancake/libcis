// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_helpers_ref_1).
// main -> test_utilities_function_objects_refwrap_refwrap_helpers_ref_1; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <functional>

// reference_wrapper

// template <ObjectType T> reference_wrapper<T> ref(T& t);

#include <functional>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_refwrap_refwrap_helpers_ref_1 { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool test()
{
  int i = 0;
  std::reference_wrapper<int> r = std::ref(i);
  assert(&r.get() == &i);
  return true;
}
} using namespace libcis_ns_utilities_function_objects_refwrap_refwrap_helpers_ref_1; // libcis


void test_utilities_function_objects_refwrap_refwrap_helpers_ref_1()
{
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
