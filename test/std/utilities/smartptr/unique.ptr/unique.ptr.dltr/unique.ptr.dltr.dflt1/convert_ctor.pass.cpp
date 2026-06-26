// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor_143d024f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// default_delete[]

// template <class U>
//   constexpr default_delete(const default_delete<U[]>&); // constexpr since C++23
//
// This constructor shall not participate in overload resolution unless
//   U(*)[] is convertible to T(*)[].

#include <memory>
#include <cassert>

#include "test_macros.h"

TEST_CONSTEXPR_CXX23 namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor_143d024f { // libcis
bool test() {
  std::default_delete<int[]> d1;
  std::default_delete<const int[]> d2 = d1;
  ((void)d2);

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
} // libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor_143d024f (libcis)

