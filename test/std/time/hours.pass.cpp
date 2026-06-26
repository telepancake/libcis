// transferred+adapted from libc++ by tools/transfer.py (slug=time_hours_911bb4a9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// typedef duration<signed integral type of at least 23 bits, ratio<3600>> hours;

#include <chrono>
#include <limits>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_hours_911bb4a9 { // libcis
int main(int, char**)
{
    typedef std::chrono::hours D;
    typedef D::rep Rep;
    typedef D::period Period;
    static_assert(std::is_signed<Rep>::value, "");
    static_assert(std::is_integral<Rep>::value, "");
    static_assert(std::numeric_limits<Rep>::digits >= 22, "");
    static_assert((std::is_same<Period, std::ratio<3600> >::value), "");

  return 0;

    return 0;
}
} // libcis_ns_time_hours_911bb4a9 (libcis)

