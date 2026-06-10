// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pair_piecewise_piecewise_construct_t).
// main -> test_utilities_utility_pairs_pair_piecewise_piecewise_construct_t; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <utility>

// struct piecewise_construct_t { explicit piecewise_construct_t() = default; };
// constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t();

#include <utility>


void test_utilities_utility_pairs_pair_piecewise_piecewise_construct_t() {
    std::piecewise_construct_t x = std::piecewise_construct;
    (void)x;

    return;
}
