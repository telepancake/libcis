// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_istream_view_general).
// main -> test_ranges_range_factories_range_istream_view_general; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization
// UNSUPPORTED: c++03, c++11, c++14, c++17

// Some basic examples of how istream_view might be used in the wild. This is a general
// collection of sample algorithms and functions that try to mock general usage of
// this view.

#include <algorithm>
#include <cassert>
#include <ranges>
#include <sstream>

#include "test_macros.h"
#include "utils.h"

namespace libcis_ns_ranges_range_factories_range_istream_view_general { // libcis: isolate file-scope helpers
template <class CharT>
void test() {
  auto ints      = make_string_stream<CharT>("0 1  2   3     4");
  auto oss       = std::basic_ostringstream<CharT>{};
  auto delimiter = make_string<CharT>("-");
  std::ranges::copy(
      std::ranges::basic_istream_view<int, CharT>(ints), std::ostream_iterator<int, CharT>{oss, delimiter.c_str()});
  auto expected = make_string<CharT>("0-1-2-3-4-");
  assert(oss.str() == expected);
}
} using namespace libcis_ns_ranges_range_factories_range_istream_view_general; // libcis


void test_ranges_range_factories_range_istream_view_general() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return;
}
