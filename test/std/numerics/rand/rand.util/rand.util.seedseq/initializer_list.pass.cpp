// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_util_rand_util_seedseq_initializer_list).
// main -> test_numerics_rand_rand_util_rand_util_seedseq_initializer_list; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <random>

// class seed_seq;

// template<class T>
//     seed_seq(initializer_list<T> il);

#include <random>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_util_rand_util_seedseq_initializer_list()
{
    std::seed_seq s= {5, 4, 3, 2, 1};
    assert(s.size() == 5);
    unsigned b[5] = {0};
    s.param(b);
    assert(b[0] == 5);
    assert(b[1] == 4);
    assert(b[2] == 3);
    assert(b[3] == 2);
    assert(b[4] == 1);

  return;
}
