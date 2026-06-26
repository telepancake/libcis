// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_ranlux48_base_dd224b26).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef subtract_with_carry_engine<uint_fast64_t, 48,  5, 12>  ranlux48_base;

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_predef_ranlux48_base_dd224b26 { // libcis
int main(int, char**)
{
    std::ranlux48_base e;
    e.discard(9999);
    assert(e() == 61839128582725ull);

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_predef_ranlux48_base_dd224b26 (libcis)

