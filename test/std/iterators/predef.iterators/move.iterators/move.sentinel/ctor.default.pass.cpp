// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_move_sentinel_ctor_default).
// main -> test_iterators_predef_iterators_move_iterators_move_sentinel_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <iterator>

// move_sentinel

// constexpr move_sentinel();

#include <iterator>
#include <cassert>

namespace libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_ctor_default { // libcis: isolate file-scope helpers
constexpr bool test()
{

  // The underlying sentinel is an integer.
  {
    std::move_sentinel<int> m;
    assert(m.base() == 0);
  }

  // The underlying sentinel is a pointer.
  {
    std::move_sentinel<int*> m;
    assert(m.base() == nullptr);
  }

  // The underlying sentinel is a user-defined type with an explicit default constructor.
  {
    struct S {
      explicit S() = default;
      int i = 3;
    };
    std::move_sentinel<S> m;
    assert(m.base().i == 3);
  }

  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_ctor_default; // libcis


void test_iterators_predef_iterators_move_iterators_move_sentinel_ctor_default()
{
  test();
  static_assert(test());

  return;
}
