// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_istream_view_iterator_increment).
// main -> test_ranges_range_factories_range_istream_view_iterator_increment; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization
// UNSUPPORTED: c++03, c++11, c++14, c++17

// iterator& operator++();
// void operator++(int);

#include <cassert>
#include <ranges>
#include <sstream>

#include "test_macros.h"
#include "../utils.h"

namespace libcis_ns_ranges_range_factories_range_istream_view_iterator_increment { // libcis: isolate file-scope helpers
template <class CharT>
void test() {
  // operator ++()
  {
    auto iss = make_string_stream<CharT>("1 2 345 ");
    std::ranges::basic_istream_view<int, CharT> isv{iss};
    auto it = isv.begin();
    assert(*it == 1);

    std::same_as<decltype(it)&> decltype(auto) it2 = ++it;
    assert(&it2 == &it);
    assert(*it2 == 2);

    ++it2;
    assert(*it2 == 345);

    ++it2;
    assert(it2 == isv.end());
  }

  // operator ++(int)
  {
    auto iss = make_string_stream<CharT>("1 2 345 ");
    std::ranges::basic_istream_view<int, CharT> isv{iss};
    auto it = isv.begin();
    assert(*it == 1);

    static_assert(std::same_as<decltype(it++), void>);
    it++;
    assert(*it == 2);
  }
}
} using namespace libcis_ns_ranges_range_factories_range_istream_view_iterator_increment; // libcis


void test_ranges_range_factories_range_istream_view_iterator_increment() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return;
}
