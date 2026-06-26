// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_ranlux24_base_7936c4ac).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef subtract_with_carry_engine<uint_fast32_t, 24, 10, 24>  ranlux24_base;

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_predef_ranlux24_base_7936c4ac { // libcis
int main(int, char**)
{
    std::ranlux24_base e;
    e.discard(9999);
    assert(e() == 7937952u);

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_predef_ranlux24_base_7936c4ac (libcis)

