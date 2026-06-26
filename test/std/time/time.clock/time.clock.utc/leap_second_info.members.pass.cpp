// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_utc_leap_second_info_members_f326d763).
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

// struct leap_second_info {
//   bool    is_leap_second;
//   seconds elapsed;
// };

#include <chrono>
#include <type_traits>

// Validates whether:
// - The members are present as non-const members.
// - The struct is an aggregate.
namespace libcis_ns_time_time_clock_time_clock_utc_leap_second_info_members_f326d763 { // libcis
int main(int, const char**) {
  static_assert(std::is_aggregate_v<std::chrono::leap_second_info>);

  std::chrono::leap_second_info leap_second_info{.is_leap_second = false, .elapsed = std::chrono::seconds(0)};

  [[maybe_unused]] bool& is_leap_second          = leap_second_info.is_leap_second;
  [[maybe_unused]] std::chrono::seconds& elapsed = leap_second_info.elapsed;

  return 0;

    return 0;
}
} // libcis_ns_time_time_clock_time_clock_utc_leap_second_info_members_f326d763 (libcis)

