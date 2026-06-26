// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_minstd_rand0_60c57739).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef linear_congruential_engine<uint_fast32_t, 16807, 0, 2147483647>
//                                                                 minstd_rand0;

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_predef_minstd_rand0_60c57739 { // libcis
int main(int, char**)
{
    std::minstd_rand0 e;
    e.discard(9999);
    assert(e() == 1043618065u);

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_predef_minstd_rand0_60c57739 (libcis)

