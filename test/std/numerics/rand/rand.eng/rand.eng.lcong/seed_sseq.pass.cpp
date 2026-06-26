// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_lcong_seed_sseq_1adc3384).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template <class UIntType, UIntType a, UIntType c, UIntType m>
//   class linear_congruential_engine;

// template<class Sseq> void seed(Sseq& q);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_eng_rand_eng_lcong_seed_sseq_1adc3384 { // libcis
int main(int, char**)
{
    {
        unsigned a[] = {3, 5, 7};
        std::seed_seq sseq(a, a+3);
        std::linear_congruential_engine<unsigned, 5, 7, 11> e1;
        std::linear_congruential_engine<unsigned, 5, 7, 11> e2(4);
        assert(e1 != e2);
        e1.seed(sseq);
        assert(e1 == e2);
    }
    {
        unsigned a[] = {3, 5, 7, 9, 11};
        std::seed_seq sseq(a, a+5);
        typedef std::linear_congruential_engine<unsigned long long, 1, 1, 0x200000001ULL> E;
        E e1(4309005589);
        E e2(sseq);
        assert(e1 == e2);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_eng_rand_eng_lcong_seed_sseq_1adc3384 (libcis)

