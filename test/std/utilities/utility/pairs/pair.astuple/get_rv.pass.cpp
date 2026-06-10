// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pair_astuple_get_rv).
// main -> test_utilities_utility_pairs_pair_astuple_get_rv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <utility>

// template <class T1, class T2> struct pair

// template<size_t I, class T1, class T2>
//     typename tuple_element<I, std::pair<T1, T2> >::type&&
//     get(pair<T1, T2>&&);

#include <utility>
#include <memory>
#include <cassert>

#include "test_macros.h"

void test_utilities_utility_pairs_pair_astuple_get_rv()
{
    {
        typedef std::pair<std::unique_ptr<int>, short> P;
        P p(std::unique_ptr<int>(new int(3)), static_cast<short>(4));
        std::unique_ptr<int> ptr = std::get<0>(std::move(p));
        assert(*ptr == 3);
    }

  return;
}
