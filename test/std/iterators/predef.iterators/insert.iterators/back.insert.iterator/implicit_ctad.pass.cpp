// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_back_insert_iterator_implicit_ctad).
// main -> test_iterators_predef_iterators_insert_iterators_back_insert_iterator_implicit_ctad; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <iterator>

// back_insert_iterator

// Make sure that the implicitly-generated CTAD works.

#include <iterator>
#include <string>
#include <vector>

#include "test_macros.h"

void test_iterators_predef_iterators_insert_iterators_back_insert_iterator_implicit_ctad() {
  {
    std::string s;
    std::back_insert_iterator it(s);
    ASSERT_SAME_TYPE(decltype(it), std::back_insert_iterator<std::string>);
  }
  {
    std::vector<int> v;
    std::back_insert_iterator it(v);
    std::back_insert_iterator copy(it);
    ASSERT_SAME_TYPE(decltype(it), std::back_insert_iterator<std::vector<int>>);
    ASSERT_SAME_TYPE(decltype(copy), std::back_insert_iterator<std::vector<int>>);
  }

  return;
}
