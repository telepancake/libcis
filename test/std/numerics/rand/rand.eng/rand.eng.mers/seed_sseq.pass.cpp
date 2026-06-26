// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_mers_seed_sseq_0afa367d).
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

// template<class Sseq> void seed(Sseq& q);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_eng_rand_eng_mers_seed_sseq_0afa367d { // libcis
void
test1()
{
    unsigned a[] = {3, 5, 7};
    std::seed_seq sseq(a, a+3);
    std::mt19937 e1;
    std::mt19937 e2(sseq);
    assert(e1 != e2);
    e1.seed(sseq);
    assert(e1 == e2);
}

void
test2()
{
    unsigned a[] = {3, 5, 7};
    std::seed_seq sseq(a, a+3);
    std::mt19937_64 e1;
    std::mt19937_64 e2(sseq);
    assert(e1 != e2);
    e1.seed(sseq);
    assert(e1 == e2);
}

int main(int, char**)
{
    test1();
    test2();

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_eng_rand_eng_mers_seed_sseq_0afa367d (libcis)

