// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_declval_declval).
// main -> test_utilities_utility_declval_declval; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <utility>

// template <class T> typename add_rvalue_reference<T>::type declval() noexcept;

#include <utility>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_declval_declval { // libcis: isolate file-scope helpers
class A
{
    A(const A&);
    A& operator=(const A&);
};
} using namespace libcis_ns_utilities_utility_declval_declval; // libcis


void test_utilities_utility_declval_declval()
{
    static_assert((std::is_same<decltype(std::declval<A>()), A&&>::value), "");

  return;
}
