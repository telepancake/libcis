// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_nonmember_make_reverse_iterator).
// main -> test_iterators_predef_iterators_reverse_iterators_reverse_iter_nonmember_make_reverse_iterator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <iterator>

// reverse_iterator

// template <class Iterator>
// reverse_iterator<Iterator> make_reverse_iterator(Iterator i); // constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_nonmember_make_reverse_iterator { // libcis: isolate file-scope helpers
template <class It>
TEST_CONSTEXPR_CXX17 void test_one(It i) {
  const std::reverse_iterator<It> r = std::make_reverse_iterator(i);
  assert(r.base() == i);
}

template <class It>
TEST_CONSTEXPR_CXX17 void test() {
  const char* s = "1234567890";
  It b(s);
  It e(s + 10);
  while (b != e)
    test_one(b++);
}

TEST_CONSTEXPR_CXX17 bool tests() {
  test<const char*>();
  test<bidirectional_iterator<const char*>>();
  test<random_access_iterator<const char*>>();
#if TEST_STD_VER >= 20
  test<cpp20_random_access_iterator<const char*>>();
#endif
  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_nonmember_make_reverse_iterator; // libcis


void test_iterators_predef_iterators_reverse_iterators_reverse_iter_nonmember_make_reverse_iterator() {
  tests();
#if TEST_STD_VER > 14
  static_assert(tests(), "");
#endif
  return;
}
