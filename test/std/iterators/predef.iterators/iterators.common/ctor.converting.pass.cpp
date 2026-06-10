// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_iterators_common_ctor_converting).
// main -> test_iterators_predef_iterators_iterators_common_ctor_converting; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class I2, class S2>
//   requires convertible_to<const I2&, I> && convertible_to<const S2&, S>
//     constexpr common_iterator(const common_iterator<I2, S2>& x);

#include <cassert>
#include <iterator>
#include <type_traits>

namespace libcis_ns_iterators_predef_iterators_iterators_common_ctor_converting { // libcis: isolate file-scope helpers
constexpr bool test()
{
  struct Base {};
  struct Derived : Base {};

  using BaseIt = std::common_iterator<Base*, const Base*>;
  using DerivedIt = std::common_iterator<Derived*, const Derived*>;
  static_assert(std::is_convertible_v<DerivedIt, BaseIt>); // Derived* to Base*
  static_assert(!std::is_constructible_v<DerivedIt, BaseIt>); // Base* to Derived*

  Derived a[10] = {};
  DerivedIt it = DerivedIt(a); // the iterator type
  BaseIt jt = BaseIt(it);
  assert(jt == BaseIt(a));

  it = DerivedIt((const Derived*)a); // the sentinel type
  jt = BaseIt(it);
  assert(jt == BaseIt(a));

  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_iterators_common_ctor_converting; // libcis


void test_iterators_predef_iterators_iterators_common_ctor_converting() {
  test();
  static_assert(test());

  return;
}
