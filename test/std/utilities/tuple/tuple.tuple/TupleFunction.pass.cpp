// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_TupleFunction).
// main -> test_utilities_tuple_tuple_tuple_TupleFunction; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// This is for bugs 18853 and 19118

#include <tuple>
#include <functional>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_TupleFunction { // libcis: isolate file-scope helpers
struct X
{
    X() {}

    template <class T>
    X(T);

    void operator()() {}
};
} using namespace libcis_ns_utilities_tuple_tuple_tuple_TupleFunction; // libcis


void test_utilities_tuple_tuple_tuple_TupleFunction()
{
    X x;
    std::function<void()> f(x);

  return;
}
