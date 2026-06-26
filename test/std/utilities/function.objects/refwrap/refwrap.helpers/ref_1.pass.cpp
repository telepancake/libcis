// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_helpers_ref_1_02068941).
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

namespace libcis_ns_utilities_function_objects_refwrap_refwrap_helpers_ref_1_02068941 { // libcis
TEST_CONSTEXPR_CXX20 bool test()
{
  int i = 0;
  std::reference_wrapper<int> r = std::ref(i);
  assert(&r.get() == &i);
  return true;
}

int main(int, char**)
{
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_refwrap_refwrap_helpers_ref_1_02068941 (libcis)

