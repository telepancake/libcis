// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_iterators_common_ctor_default).
// main -> test_iterators_predef_iterators_iterators_common_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr common_iterator() requires default_initializable<I> = default;

#include <cassert>
#include <iterator>
#include <type_traits>

#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_iterators_common_ctor_default { // libcis: isolate file-scope helpers
constexpr bool test()
{
  {
    using It = cpp17_input_iterator<int*>;
    using CommonIt = std::common_iterator<It, sentinel_wrapper<It>>;
    static_assert(!std::is_default_constructible_v<It>); // premise
    static_assert(!std::is_default_constructible_v<CommonIt>); // conclusion
  }
  {
    // The base iterator is value-initialized.
    std::common_iterator<int*, sentinel_wrapper<int*>> c;
    assert(c == nullptr);
  }

  return true;
}
} using namespace libcis_ns_iterators_predef_iterators_iterators_common_ctor_default; // libcis


void test_iterators_predef_iterators_iterators_common_ctor_default() {
  test();
  static_assert(test());

  return;
}
