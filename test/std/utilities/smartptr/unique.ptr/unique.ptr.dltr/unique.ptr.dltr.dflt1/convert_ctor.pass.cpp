// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX23 bool test() {
  std::default_delete<int[]> d1;
  std::default_delete<const int[]> d2 = d1;
  ((void)d2);

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_dltr_unique_ptr_dltr_dflt1_convert_ctor() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
