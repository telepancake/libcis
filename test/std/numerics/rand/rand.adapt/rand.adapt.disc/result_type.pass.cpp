// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_disc_result_type_f8a3432b).
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
// {
// public:
//     // types
//     typedef typename Engine::result_type result_type;

#include <random>
#include <cstdint>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_disc_result_type_f8a3432b { // libcis
void
test1()
{
    static_assert((std::is_same<
        std::ranlux24::result_type,
        std::uint_fast32_t>::value), "");
}

void
test2()
{
    static_assert((std::is_same<
        std::ranlux48::result_type,
        std::uint_fast64_t>::value), "");
}

int main(int, char**)
{
    test1();
    test2();

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_adapt_rand_adapt_disc_result_type_f8a3432b (libcis)

