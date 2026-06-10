// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_iterators_crbegin).
// main -> test_strings_basic_string_string_iterators_crbegin; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// const_reverse_iterator crbegin() const; // constexpr since C++20

#include <string>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_strings_basic_string_string_iterators_crbegin { // libcis: isolate file-scope helpers
template <class S>
TEST_CONSTEXPR_CXX20 void test(const S& s) {
  typename S::const_reverse_iterator cb = s.crbegin();
  if (!s.empty()) {
    assert(*cb == s.back());
  }
  assert(cb == s.rbegin());
}

template <class S>
TEST_CONSTEXPR_CXX20 void test_string() {
  test(S());
  test(S("123"));
}

TEST_CONSTEXPR_CXX20 bool test() {
  test_string<std::string>();
#if TEST_STD_VER >= 11
  test_string<std::basic_string<char, std::char_traits<char>, min_allocator<char> > >();
#endif

  return true;
}
} using namespace libcis_ns_strings_basic_string_string_iterators_crbegin; // libcis


void test_strings_basic_string_string_iterators_crbegin() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
