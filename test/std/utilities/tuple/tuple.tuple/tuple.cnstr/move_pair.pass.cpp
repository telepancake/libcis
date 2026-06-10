// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_cnstr_move_pair).
// main -> test_utilities_tuple_tuple_tuple_tuple_cnstr_move_pair; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template <class U1, class U2> tuple(pair<U1, U2>&& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_cnstr_move_pair { // libcis: isolate file-scope helpers
struct B
{
    int id_;

    explicit B(int i) : id_(i) {}

    virtual ~B() {}
};

struct D
    : B
{
    explicit D(int i) : B(i) {}
};
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_cnstr_move_pair; // libcis


void test_utilities_tuple_tuple_tuple_tuple_cnstr_move_pair()
{
    {
        typedef std::pair<long, std::unique_ptr<D>> T0;
        typedef std::tuple<long long, std::unique_ptr<B>> T1;
        T0 t0(2, std::unique_ptr<D>(new D(3)));
        T1 t1 = std::move(t0);
        assert(std::get<0>(t1) == 2);
        assert(std::get<1>(t1)->id_ == 3);
    }

#if TEST_STD_VER > 11
    {
        using pair_t = std::pair<int, char>;
        constexpr std::tuple<long, long> t(pair_t(0, 'a'));
        static_assert(std::get<0>(t) == 0, "");
        static_assert(std::get<1>(t) == 'a', "");
    }
#endif

  return;
}
