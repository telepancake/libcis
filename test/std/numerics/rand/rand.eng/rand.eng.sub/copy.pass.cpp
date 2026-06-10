// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_sub_copy).
// main -> test_numerics_rand_rand_eng_rand_eng_sub_copy; file-scope helpers isolated in anon namespace.
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

// subtract_with_carry_engine(const subtract_with_carry_engine&);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::ranlux24_base E;
    E e1;
    (void)e1();
    E e2 = e1;
    assert(e1 == e2);
    assert(e1() == e2());
    E::result_type k = e1();
    assert(e1 != e2);
    assert(e2() == k);
    assert(e1 == e2);
}

void
test2()
{
    typedef std::ranlux48_base E;
    E e1;
    (void)e1();
    E e2(e1);
    assert(e1 == e2);
    assert(e1() == e2());
    E::result_type k = e1();
    assert(e1 != e2);
    assert(e2() == k);
    assert(e1 == e2);
}
} // anonymous namespace (libcis)


void test_numerics_rand_rand_eng_rand_eng_sub_copy()
{
    test1();
    test2();

  return;
}
