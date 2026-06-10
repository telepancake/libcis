// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_istream_view_iterator_compare).
// main -> test_ranges_range_factories_range_istream_view_iterator_compare; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization
// UNSUPPORTED: c++03, c++11, c++14, c++17

// friend bool operator==(const iterator& x, default_sentinel_t);

#include <cassert>
#include <ranges>
#include <sstream>

#include "test_macros.h"
#include "../utils.h"

namespace libcis_ns_ranges_range_factories_range_istream_view_iterator_compare { // libcis: isolate file-scope helpers
template <class CharT>
void test() {
  // fail to read
  {
    auto iss = make_string_stream<CharT>("a123 4");
    std::ranges::basic_istream_view<int, CharT> isv{iss};
    auto it = isv.begin();
    assert(it == std::default_sentinel);
  }

  // iterate through the end
  {
    auto iss = make_string_stream<CharT>("123 ");
    std::ranges::basic_istream_view<int, CharT> isv{iss};
    auto it = isv.begin();
    assert(it != std::default_sentinel);
    ++it;
    assert(it == std::default_sentinel);
  }

  // empty stream
  {
    auto iss = make_string_stream<CharT>("");
    std::ranges::basic_istream_view<int, CharT> isv{iss};
    auto it = isv.begin();
    assert(it == std::default_sentinel);
  }
}
} using namespace libcis_ns_ranges_range_factories_range_istream_view_iterator_compare; // libcis


void test_ranges_range_factories_range_istream_view_iterator_compare() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return;
}
