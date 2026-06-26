// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_declval_declval_39fb8e9a).
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

namespace libcis_ns_utilities_utility_declval_declval_39fb8e9a { // libcis
class A
{
    A(const A&);
    A& operator=(const A&);
};

int main(int, char**)
{
    static_assert((std::is_same<decltype(std::declval<A>()), A&&>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_declval_declval_39fb8e9a (libcis)

