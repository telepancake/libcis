// transferred+adapted from libc++ by tools/transfer.py (slug=time_minutes_0835f527).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// typedef duration<signed integral type of at least 29 bits, ratio< 60>> minutes;

#include <chrono>
#include <limits>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_minutes_0835f527 { // libcis
int main(int, char**)
{
    typedef std::chrono::minutes D;
    typedef D::rep Rep;
    typedef D::period Period;
    static_assert(std::is_signed<Rep>::value, "");
    static_assert(std::is_integral<Rep>::value, "");
    static_assert(std::numeric_limits<Rep>::digits >= 28, "");
    static_assert((std::is_same<Period, std::ratio<60> >::value), "");

  return 0;

    return 0;
}
} // libcis_ns_time_minutes_0835f527 (libcis)

