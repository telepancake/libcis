// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_zone_time_zone_timezone_time_zone_nonmembers_comparison_41e9467e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-filesystem, no-localization, no-tzdb

// XFAIL: libcpp-has-no-experimental-tzdb
// XFAIL: availability-tzdb-missing

// <chrono>

// bool operator==(const time_zone& x, const time_zone& y) noexcept;
// strong_ordering operator<=>(const time_zone& x, const time_zone& y) noexcept;

#include <chrono>
#include <cassert>

#include "test_macros.h"
#include "test_comparisons.h"

namespace libcis_ns_time_time_zone_time_zone_timezone_time_zone_nonmembers_comparison_41e9467e { // libcis
int main(int, const char**) {
  const std::chrono::tzdb& tzdb = std::chrono::get_tzdb();
  assert(tzdb.zones.size() > 2);

  AssertOrderAreNoexcept<std::chrono::time_zone>();
  AssertOrderReturn<std::strong_ordering, std::chrono::time_zone>();

  assert(testOrder(tzdb.zones[0], tzdb.zones[1], std::strong_ordering::less));

  return 0;

    return 0;
}
} // libcis_ns_time_time_zone_time_zone_timezone_time_zone_nonmembers_comparison_41e9467e (libcis)

