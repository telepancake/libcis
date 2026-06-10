// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_mers_eval).
// main -> test_numerics_rand_rand_eng_rand_eng_mers_eval; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template <class UIntType, size_t w, size_t n, size_t m, size_t r,
//           UIntType a, size_t u, UIntType d, size_t s,
//           UIntType b, size_t t, UIntType c, size_t l, UIntType f>
// class mersenne_twister_engine;

// result_type operator()();

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_eng_rand_eng_mers_eval { // libcis: isolate file-scope helpers
void
test1()
{
    std::mt19937 e;
    assert(e() == 3499211612u);
    assert(e() == 581869302u);
    assert(e() == 3890346734u);
}

void
test2()
{
    std::mt19937_64 e;
    assert(e() == 14514284786278117030ull);
    assert(e() == 4620546740167642908ull);
    assert(e() == 13109570281517897720ull);
}
} using namespace libcis_ns_numerics_rand_rand_eng_rand_eng_mers_eval; // libcis


void test_numerics_rand_rand_eng_rand_eng_mers_eval()
{
    test1();
    test2();

  return;
}
