// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_eng_rand_eng_sub_result_type).
// main -> test_numerics_rand_rand_eng_rand_eng_sub_result_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <random>

// template<class UIntType, size_t w, size_t s, size_t r>
// class subtract_with_carry_engine
// {
// public:
//     // types
//     typedef UIntType result_type;

#include <random>
#include <cstdint>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_eng_rand_eng_sub_result_type { // libcis: isolate file-scope helpers
void
test1()
{
    static_assert((std::is_same<
        std::ranlux24_base::result_type,
        std::uint_fast32_t>::value), "");
}

void
test2()
{
    static_assert((std::is_same<
        std::ranlux48_base::result_type,
        std::uint_fast64_t>::value), "");
}
} using namespace libcis_ns_numerics_rand_rand_eng_rand_eng_sub_result_type; // libcis


void test_numerics_rand_rand_eng_rand_eng_sub_result_type()
{
    test1();
    test2();

  return;
}
