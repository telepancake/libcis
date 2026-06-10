// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_PR38601).
// main -> test_utilities_tuple_tuple_tuple_PR38601; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Regression test for https://llvm.org/PR38601

// UNSUPPORTED: c++03

#include <cassert>
#include <tuple>

namespace libcis_ns_utilities_tuple_tuple_tuple_PR38601 { // libcis: isolate file-scope helpers
using Base = std::tuple<int, int>;

struct Derived : Base {
    template <class ...Ts>
    Derived(int x, Ts... ts): Base(ts...), x_(x) { }
    operator int () const { return x_; }
    int x_;
};
} using namespace libcis_ns_utilities_tuple_tuple_tuple_PR38601; // libcis


void test_utilities_tuple_tuple_tuple_PR38601() {
    Derived d(1, 2, 3);
    Base b = static_cast<Base>(d);
    assert(std::get<0>(b) == 2);
    assert(std::get<1>(b) == 3);
    return;
}
