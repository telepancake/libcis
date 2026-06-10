// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_modifiers_string_op_plus_equal_initializer_list).
// main -> test_strings_basic_string_string_modifiers_string_op_plus_equal_initializer_list; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <string>

// basic_string& operator+=(initializer_list<charT> il); // constexpr since C++20

#include <string>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_strings_basic_string_string_modifiers_string_op_plus_equal_initializer_list { // libcis: isolate file-scope helpers
template <class S>
TEST_CONSTEXPR_CXX20 void test_string() {
  S s("123");
  s += {'a', 'b', 'c'};
  assert(s == "123abc");
  LIBCPP_ASSERT(is_string_asan_correct(s));

  s += {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  assert(s == "123abcxxxxxxxxxxxxxxxx");
  LIBCPP_ASSERT(is_string_asan_correct(s));

  s += {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  assert(s == "123abcxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  LIBCPP_ASSERT(is_string_asan_correct(s));

  s += {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
        'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
        'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
  assert(s == "123abcxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  LIBCPP_ASSERT(is_string_asan_correct(s));
}

TEST_CONSTEXPR_CXX20 bool test() {
  test_string<std::string>();
  test_string<std::basic_string<char, std::char_traits<char>, min_allocator<char> > >();
  test_string<std::basic_string<char, std::char_traits<char>, safe_allocator<char> > >();
  return true;
}
} using namespace libcis_ns_strings_basic_string_string_modifiers_string_op_plus_equal_initializer_list; // libcis


void test_strings_basic_string_string_modifiers_string_op_plus_equal_initializer_list() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
