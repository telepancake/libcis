// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_disc_ctor_engine_copy_c2ee4b10).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class Engine, size_t p, size_t r>
// class discard_block_engine

// explicit discard_block_engine(const Engine& e);

#include <random>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_disc_ctor_engine_copy_c2ee4b10 { // libcis
int main(int, char**)
{
    {
        typedef std::ranlux24_base Engine;
        typedef std::ranlux24 Adaptor;
        Engine e;
        Adaptor a(e);
        assert(a.base() == e);
    }

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_adapt_rand_adapt_disc_ctor_engine_copy_c2ee4b10 (libcis)

