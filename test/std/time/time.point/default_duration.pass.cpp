// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_point_default_duration_e6d38f61).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// time_point

// Test default template arg:

// template <class Clock, class Duration = typename Clock::duration>
//   class time_point;

#include <chrono>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_time_point_default_duration_e6d38f61 { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::chrono::system_clock::duration,
                   std::chrono::time_point<std::chrono::system_clock>::duration>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_time_time_point_default_duration_e6d38f61 (libcis)

