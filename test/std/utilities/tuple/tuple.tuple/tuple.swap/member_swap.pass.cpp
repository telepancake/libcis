// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_swap_member_swap).
// main -> test_utilities_tuple_tuple_tuple_tuple_swap_member_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// void swap(tuple& rhs);

// UNSUPPORTED: c++03

#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_swap_member_swap { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20
bool test()
{
    {
        typedef std::tuple<> T;
        T t0;
        T t1;
        t0.swap(t1);
    }
    {
        typedef std::tuple<MoveOnly> T;
        T t0(MoveOnly(0));
        T t1(MoveOnly(1));
        t0.swap(t1);
        assert(std::get<0>(t0) == 1);
        assert(std::get<0>(t1) == 0);
    }
    {
        typedef std::tuple<MoveOnly, MoveOnly> T;
        T t0(MoveOnly(0), MoveOnly(1));
        T t1(MoveOnly(2), MoveOnly(3));
        t0.swap(t1);
        assert(std::get<0>(t0) == 2);
        assert(std::get<1>(t0) == 3);
        assert(std::get<0>(t1) == 0);
        assert(std::get<1>(t1) == 1);
    }
    {
        typedef std::tuple<MoveOnly, MoveOnly, MoveOnly> T;
        T t0(MoveOnly(0), MoveOnly(1), MoveOnly(2));
        T t1(MoveOnly(3), MoveOnly(4), MoveOnly(5));
        t0.swap(t1);
        assert(std::get<0>(t0) == 3);
        assert(std::get<1>(t0) == 4);
        assert(std::get<2>(t0) == 5);
        assert(std::get<0>(t1) == 0);
        assert(std::get<1>(t1) == 1);
        assert(std::get<2>(t1) == 2);
    }
    return true;
}
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_swap_member_swap; // libcis


void test_utilities_tuple_tuple_tuple_tuple_swap_member_swap()
{
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return;
}
