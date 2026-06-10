// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_ibits_default).
// main -> test_numerics_rand_rand_adapt_rand_adapt_ibits_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class Engine, size_t w, class UIntType>
// class independent_bits_engine

// explicit independent_bits_engine();

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_ibits_default { // libcis: isolate file-scope helpers
void
test1()
{
    std::independent_bits_engine<std::ranlux24, 32, unsigned> e1;
    std::independent_bits_engine<std::ranlux24, 32, unsigned> e2(std::ranlux24_base::default_seed);
    assert(e1 == e2);
    assert(e1() == 2066486613);
}

void
test2()
{
    std::independent_bits_engine<std::ranlux48, 64, unsigned long long> e1;
    std::independent_bits_engine<std::ranlux48, 64, unsigned long long> e2(std::ranlux48_base::default_seed);
    assert(e1 == e2);
    assert(e1() == 18223106896348967647ull);
}
} using namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_ibits_default; // libcis


void test_numerics_rand_rand_adapt_rand_adapt_ibits_default()
{
    test1();
    test2();

  return;
}
