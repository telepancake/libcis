// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_lcong_ctor_sseq_43b498ec).
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

// template<class Sseq> explicit linear_congruential_engine(Sseq& q);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_eng_rand_eng_lcong_ctor_sseq_43b498ec { // libcis
int main(int, char**)
{
    {
        unsigned a[] = {3, 5, 7};
        std::seed_seq sseq(a, a+3);
        std::linear_congruential_engine<unsigned, 5, 7, 11> e1(sseq);
        std::linear_congruential_engine<unsigned, 5, 7, 11> e2(4);
        assert(e1 == e2);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_eng_rand_eng_lcong_ctor_sseq_43b498ec (libcis)

