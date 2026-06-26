// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_zone_time_zone_db_time_zone_db_remote_remote_version_c672ead4).
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

// const string remote_version();

#include <chrono>
#include <cassert>
#include <string>

#include "test_macros.h"

namespace libcis_ns_time_time_zone_time_zone_db_time_zone_db_remote_remote_version_c672ead4 { // libcis
int main(int, const char**) {
  std::string version = std::chrono::remote_version();
  assert(!version.empty());

  assert(version == std::chrono::get_tzdb().version);
  assert(version == std::chrono::get_tzdb_list().front().version);

  return 0;

    return 0;
}
} // libcis_ns_time_time_zone_time_zone_db_time_zone_db_remote_remote_version_c672ead4 (libcis)

