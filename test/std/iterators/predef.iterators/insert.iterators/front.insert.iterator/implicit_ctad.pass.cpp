// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_front_insert_iterator_implicit_ctad_c281c3c8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <iterator>

// front_insert_iterator

// Make sure that the implicitly-generated CTAD works.

#include <deque>
#include <iterator>
#include <string>

#include "test_macros.h"

namespace libcis_ns_iterators_predef_iterators_insert_iterators_front_insert_iterator_implicit_ctad_c281c3c8 { // libcis
int main(int, char**) {
  {
    std::string s;
    std::front_insert_iterator it(s);
    ASSERT_SAME_TYPE(decltype(it), std::front_insert_iterator<std::string>);
  }
  {
    std::deque<int> v;
    std::front_insert_iterator it(v);
    std::front_insert_iterator copy(it);
    ASSERT_SAME_TYPE(decltype(it), std::front_insert_iterator<std::deque<int>>);
    ASSERT_SAME_TYPE(decltype(copy), std::front_insert_iterator<std::deque<int>>);
  }

  return 0;

    return 0;
}
} // libcis_ns_iterators_predef_iterators_insert_iterators_front_insert_iterator_implicit_ctad_c281c3c8 (libcis)

