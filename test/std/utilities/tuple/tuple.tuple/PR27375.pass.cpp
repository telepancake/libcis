// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_PR27375).
// main -> test_utilities_tuple_tuple_tuple_PR27375; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Regression test for https://llvm.org/PR27375

// UNSUPPORTED: c++03

#include <tuple>

void test_utilities_tuple_tuple_tuple_PR27375() {
    std::tuple<int&>(std::tuple<int&&>(42));

    return;
}
