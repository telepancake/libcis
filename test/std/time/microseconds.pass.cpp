// transferred+adapted from libc++ by tools/transfer.py (slug=time_microseconds_afbd2c38).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// typedef duration<signed integral type of at least 55 bits, micro> microseconds;

#include <chrono>
#include <limits>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_microseconds_afbd2c38 { // libcis
int main(int, char**)
{
    typedef std::chrono::microseconds D;
    typedef D::rep Rep;
    typedef D::period Period;
    static_assert(std::is_signed<Rep>::value, "");
    static_assert(std::is_integral<Rep>::value, "");
    static_assert(std::numeric_limits<Rep>::digits >= 54, "");
    static_assert((std::is_same<Period, std::micro>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_time_microseconds_afbd2c38 (libcis)

