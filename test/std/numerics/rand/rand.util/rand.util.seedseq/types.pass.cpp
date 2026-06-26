// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_util_rand_util_seedseq_types_bf51cd24).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// class seed_seq
// {
// public:
//     // types
//     typedef uint_least32_t result_type;

#include <random>
#include <cstdint>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_util_rand_util_seedseq_types_bf51cd24 { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::seed_seq::result_type, std::uint_least32_t>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_util_rand_util_seedseq_types_bf51cd24 (libcis)

