// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_counted_iterator_implicit_ctad_e3925cef).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// counted_iterator

// Make sure that the implicitly-generated CTAD works.

#include <iterator>

#include "test_macros.h"

namespace libcis_ns_iterators_predef_iterators_counted_iterator_implicit_ctad_e3925cef { // libcis
int main(int, char**) {
  int array[] = {1, 2, 3};
  int* p = array;
  std::counted_iterator iter(p, 3);
  ASSERT_SAME_TYPE(decltype(iter), std::counted_iterator<int*>);

  return 0;

    return 0;
}
} // libcis_ns_iterators_predef_iterators_counted_iterator_implicit_ctad_e3925cef (libcis)

