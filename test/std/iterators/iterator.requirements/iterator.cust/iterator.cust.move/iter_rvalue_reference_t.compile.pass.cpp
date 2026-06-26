// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_iterator_cust_iterator_cust_move_iter_rvalue_reference_t_aef0f3dd).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class I>
// using iter_rvalue_reference;

#include <iterator>

namespace libcis_ns_iterators_iterator_requirements_iterator_cust_iterator_cust_move_iter_rvalue_reference_t_aef0f3dd { // libcis
static_assert(std::same_as<std::iter_rvalue_reference_t<int*>, int&&>);
static_assert(std::same_as<std::iter_rvalue_reference_t<const int*>, const int&&>);

void test_undefined_internal() {
  struct A {
    int& operator*() const;
  };
  static_assert(std::same_as<std::iter_rvalue_reference_t<A>, int&&>);
}
} // libcis_ns_iterators_iterator_requirements_iterator_cust_iterator_cust_move_iter_rvalue_reference_t_aef0f3dd (libcis)

