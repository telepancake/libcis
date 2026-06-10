// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_util_rand_util_seedseq_types).
// main -> test_numerics_rand_rand_util_rand_util_seedseq_types; file-scope helpers isolated in anon namespace.
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

void test_numerics_rand_rand_util_rand_util_seedseq_types()
{
    static_assert((std::is_same<std::seed_seq::result_type, std::uint_least32_t>::value), "");

  return;
}
