// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_duration_types_637866f8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// Test nested types

// typedef Rep rep;
// typedef Period period;

#include <chrono>
#include <ratio>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_time_time_duration_types_637866f8 { // libcis
int main(int, char**)
{
    typedef std::chrono::duration<long, std::ratio<3, 2> > D;
    static_assert((std::is_same<D::rep, long>::value), "");
    static_assert((std::is_same<D::period, std::ratio<3, 2> >::value), "");

  return 0;

    return 0;
}
} // libcis_ns_time_time_duration_types_637866f8 (libcis)

