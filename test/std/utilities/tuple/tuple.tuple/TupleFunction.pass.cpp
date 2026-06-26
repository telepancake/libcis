// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_TupleFunction_4b050388).
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

namespace libcis_ns_utilities_tuple_tuple_tuple_TupleFunction_4b050388 { // libcis
struct X
{
    X() {}

    template <class T>
    X(T);

    void operator()() {}
};

int main(int, char**)
{
    X x;
    std::function<void()> f(x);

  return 0;

    return 0;
}
} // libcis_ns_utilities_tuple_tuple_tuple_TupleFunction_4b050388 (libcis)

