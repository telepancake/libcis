// AST-transferred from libc++ by tools/transfer.py (slug=strings_string_view_string_view_access_back).
// main -> test_strings_string_view_string_view_access_back; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: !stdlib=libc++ && (c++03 || c++11 || c++14)

// <string_view>

// constexpr const _CharT& front();

#include <string_view>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_string_view_string_view_access_back { // libcis: isolate file-scope helpers
template <typename CharT>
bool test(const CharT* s, std::size_t len) {
  typedef std::basic_string_view<CharT> SV;
  SV sv(s, len);
  ASSERT_SAME_TYPE(decltype(sv.back()), typename SV::const_reference);
  LIBCPP_ASSERT_NOEXCEPT(sv.back());
  assert(sv.length() == len);
  assert(sv.back() == s[len - 1]);
  return &sv.back() == s + len - 1;
}
} using namespace libcis_ns_strings_string_view_string_view_access_back; // libcis


void test_strings_string_view_string_view_access_back() {
  assert(test("ABCDE", 5));
  assert(test("a", 1));

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  assert(test(L"ABCDE", 5));
  assert(test(L"a", 1));
#endif

#if TEST_STD_VER >= 11
  assert(test(u"ABCDE", 5));
  assert(test(u"a", 1));

  assert(test(U"ABCDE", 5));
  assert(test(U"a", 1));
#endif

#if TEST_STD_VER >= 11
  {
    constexpr std::basic_string_view<char> sv("ABC", 2);
    static_assert(sv.length() == 2, "");
    static_assert(sv.back() == 'B', "");
  }
#endif

  return;
}
