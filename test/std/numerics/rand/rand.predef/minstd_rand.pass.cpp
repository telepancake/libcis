// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_minstd_rand_9d953f30).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647>
//                                                                 minstd_rand;

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_predef_minstd_rand_9d953f30 { // libcis
int main(int, char**)
{
    std::minstd_rand e;
    e.discard(9999);
    assert(e() == 399268537u);

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_predef_minstd_rand_9d953f30 (libcis)

