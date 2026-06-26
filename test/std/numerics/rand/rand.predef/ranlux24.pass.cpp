// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_predef_ranlux24_20ec3b14).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// typedef discard_block_engine<ranlux24_base, 223, 23>                ranlux24;

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_predef_ranlux24_20ec3b14 { // libcis
int main(int, char**)
{
    std::ranlux24 e;
    e.discard(9999);
    assert(e() == 9901578u);

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_predef_ranlux24_20ec3b14 (libcis)

