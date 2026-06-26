// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_members_overflow_7db7eef3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class strstreambuf

// int overflow(int c);

#include <iostream>
#include <string>
#include <strstream>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_members_overflow_7db7eef3 { // libcis
int main(int, char**) {
  std::ostrstream oss;
  std::string s;

  for (int i = 0; i < 4096; ++i)
    s.push_back((i % 16) + 'a');

  oss << s << std::ends;
  std::cout << oss.str();
  oss.freeze(false);

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_members_overflow_7db7eef3 (libcis)

