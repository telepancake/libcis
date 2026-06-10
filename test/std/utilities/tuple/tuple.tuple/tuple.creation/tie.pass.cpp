// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_creation_tie).
// main -> test_utilities_tuple_tuple_tuple_tuple_creation_tie; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template<class... Types>
//   tuple<Types&...> tie(Types&... t);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_creation_tie { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX14
bool test_tie()
{
    {
        int i = 42;
        double f = 1.1;
        using ExpectT = std::tuple<int&, decltype(std::ignore)&, double&>;
        auto res = std::tie(i, std::ignore, f);
        static_assert(std::is_same<ExpectT, decltype(res)>::value, "");
        assert(&std::get<0>(res) == &i);
        assert(&std::get<1>(res) == &std::ignore);
        assert(&std::get<2>(res) == &f);

#if TEST_STD_VER >= 20
        res = std::make_tuple(101, nullptr, -1.0);
        assert(i == 101);
        assert(f == -1.0);
#endif
    }
    return true;
}
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_creation_tie; // libcis


void test_utilities_tuple_tuple_tuple_tuple_creation_tie()
{
    test_tie();
#if TEST_STD_VER >= 14
    static_assert(test_tie(), "");
#endif

    {
        int i = 0;
        std::string s;
        std::tie(i, std::ignore, s) = std::make_tuple(42, 3.14, "C++");
        assert(i == 42);
        assert(s == "C++");
    }

    return;
}
