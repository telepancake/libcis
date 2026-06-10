// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_operators_rel_ops).
// main -> test_utilities_utility_operators_rel_ops; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test rel_ops

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <utility>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_operators_rel_ops { // libcis: isolate file-scope helpers
struct A
{
    int data_;

    explicit A(int data = -1) : data_(data) {}
};

inline
bool
operator == (const A& x, const A& y)
{
    return x.data_ == y.data_;
}

inline
bool
operator < (const A& x, const A& y)
{
    return x.data_ < y.data_;
}
} using namespace libcis_ns_utilities_utility_operators_rel_ops; // libcis


void test_utilities_utility_operators_rel_ops()
{
    using namespace std::rel_ops;
    A a1(1);
    A a2(2);
    assert(a1 == a1);
    assert(a1 != a2);
    assert(a1 < a2);
    assert(a2 > a1);
    assert(a1 <= a1);
    assert(a1 <= a2);
    assert(a2 >= a2);
    assert(a2 >= a1);

  return;
}
