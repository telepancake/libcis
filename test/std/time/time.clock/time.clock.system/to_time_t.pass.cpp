// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_system_to_time_t_5c7a2b83).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// system_clock

// time_t to_time_t(const time_point& t);

#include <chrono>
#include <ctime>

#include "test_macros.h"

namespace libcis_ns_time_time_clock_time_clock_system_to_time_t_5c7a2b83 { // libcis
int main(int, char**)
{
    typedef std::chrono::system_clock C;
    std::time_t t1 = C::to_time_t(C::now());
    ((void)t1);

  return 0;

    return 0;
}
} // libcis_ns_time_time_clock_time_clock_system_to_time_t_5c7a2b83 (libcis)

