// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_zone_time_zone_link_time_zone_link_members_target_b4d21f0d).
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

// class time_zone_link;

// string_view target()   const noexcept;

#include <cassert>
#include <chrono>
#include <concepts>

#include "test_macros.h"

namespace libcis_ns_time_time_zone_time_zone_link_time_zone_link_members_target_b4d21f0d { // libcis
int main(int, const char**) {
  const std::chrono::tzdb& tzdb = std::chrono::get_tzdb();
  assert(tzdb.links.size() > 1);

  [[maybe_unused]] std::same_as<std::string_view> auto _ = tzdb.links[0].target();
  static_assert(noexcept(tzdb.links[0].target()));

  return 0;

    return 0;
}
} // libcis_ns_time_time_zone_time_zone_link_time_zone_link_members_target_b4d21f0d (libcis)

