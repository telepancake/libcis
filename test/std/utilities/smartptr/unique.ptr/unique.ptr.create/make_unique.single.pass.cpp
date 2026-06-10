// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_single).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_single; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11
#include <memory>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_single { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX23 bool test() {
  {
    std::unique_ptr<int> p1 = std::make_unique<int>(1);
    assert(*p1 == 1);
    p1 = std::make_unique<int>();
    assert(*p1 == 0);
  }

  {
    std::unique_ptr<std::string> p2 = std::make_unique<std::string>("Meow!");
    assert(*p2 == "Meow!");
    p2 = std::make_unique<std::string>();
    assert(*p2 == "");
    p2 = std::make_unique<std::string>(6, 'z');
    assert(*p2 == "zzzzzz");
  }

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_single; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_single() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
