// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_limits_limits_denorm_style_check_values).
// main -> test_language_support_support_limits_limits_denorm_style_check_values; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test numeric_limits

// float_round_style

#include <limits>

#include "test_macros.h"

namespace libcis_ns_language_support_support_limits_limits_denorm_style_check_values { // libcis: isolate file-scope helpers
typedef char one;
struct two {one _[2];};

one test(std::float_round_style);
two test(int);
} using namespace libcis_ns_language_support_support_limits_limits_denorm_style_check_values; // libcis


void test_language_support_support_limits_limits_denorm_style_check_values()
{
    static_assert(std::round_indeterminate == -1,
                 "std::round_indeterminate == -1");
    static_assert(std::round_toward_zero == 0,
                 "std::round_toward_zero == 0");
    static_assert(std::round_to_nearest == 1,
                 "std::round_to_nearest == 1");
    static_assert(std::round_toward_infinity == 2,
                 "std::round_toward_infinity == 2");
    static_assert(std::round_toward_neg_infinity == 3,
                 "std::round_toward_neg_infinity == 3");
    static_assert(sizeof(test(std::round_to_nearest)) == 1,
                 "sizeof(test(std::round_to_nearest)) == 1");
    static_assert(sizeof(test(1)) == 2,
                 "sizeof(test(1)) == 2");

  return;
}
