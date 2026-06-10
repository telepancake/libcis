// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_sub_default).
// main -> test_numerics_rand_rand_eng_rand_eng_sub_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class UIntType, size_t w, size_t s, size_t r>
// class subtract_with_carry_engine;

// explicit subtract_with_carry_engine();

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    std::ranlux24_base e1;
    std::ranlux24_base e2(std::ranlux24_base::default_seed);
    assert(e1 == e2);
    assert(e1() == 15039276);
}

void
test2()
{
    std::ranlux48_base e1;
    std::ranlux48_base e2(std::ranlux48_base::default_seed);
    assert(e1 == e2);
    assert(e1() == 23459059301164ull);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_eng_rand_eng_sub_default()
{
    test1();
    test2();

  return;
}
