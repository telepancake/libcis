// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_istream_view_end).
// main -> test_ranges_range_factories_range_istream_view_end; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization
// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr default_sentinel_t end() const noexcept;

#include <cassert>
#include <ranges>
#include <sstream>

#include "test_macros.h"
#include "utils.h"

namespace libcis_ns_ranges_range_factories_range_istream_view_end { // libcis: isolate file-scope helpers
template <class T>
concept NoexceptEnd =
    requires(T t) {
      { t.end() } noexcept;
    };

static_assert(NoexceptEnd<std::ranges::istream_view<int>>);
static_assert(NoexceptEnd<const std::ranges::istream_view<int>>);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
static_assert(NoexceptEnd<std::ranges::wistream_view<int>>);
static_assert(NoexceptEnd<const std::ranges::wistream_view<int>>);
#endif

template <class CharT>
void test() {
  auto iss = make_string_stream<CharT>("12");
  std::ranges::basic_istream_view<int, CharT> isv{iss};
  [[maybe_unused]] std::same_as<std::default_sentinel_t> auto sent = isv.end();
}
} using namespace libcis_ns_ranges_range_factories_range_istream_view_end; // libcis


void test_ranges_range_factories_range_istream_view_end() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return;
}
