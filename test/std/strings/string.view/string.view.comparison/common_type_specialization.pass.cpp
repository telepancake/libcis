// AST-transferred from libc++ by tools/transfer.py (slug=strings_string_view_string_view_comparison_common_type_specialization).
// main -> test_strings_string_view_string_view_comparison_common_type_specialization; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: !stdlib=libc++ && (c++03 || c++11 || c++14)

// During the review D130295 it was noticed libc++'s implementation uses
// std::common_type. When users specialize this template for their own types the
// comparisons would fail. This tests with a specialized std::common_type.

// <string_view>

#include <string_view>

#include <cassert>
#include <cstring>

#include "test_comparisons.h"
#include "test_macros.h"

namespace libcis_ns_strings_string_view_string_view_comparison_common_type_specialization { // libcis: isolate file-scope helpers
struct char_wrapper {
  char c;
};
} using namespace libcis_ns_strings_string_view_string_view_comparison_common_type_specialization; // libcis


template <>
struct std::char_traits<char_wrapper> {
  using char_type = char_wrapper;

  static bool eq(char_wrapper lhs, char_wrapper rhs) { return lhs.c == rhs.c; }

  static std::size_t length(const char_wrapper* a) {
    static_assert(sizeof(char_wrapper) == 1, "strlen requires this");
    return std::strlen(reinterpret_cast<const char*>(a));
  }

  static int compare(const char_wrapper* lhs, const char_wrapper* rhs, std::size_t count) {
    return std::char_traits<char>::compare(
        reinterpret_cast<const char*>(lhs), reinterpret_cast<const char*>(rhs), count);
  }
};

namespace libcis_ns_strings_string_view_string_view_comparison_common_type_specialization { // libcis: isolate file-scope helpers
using WrappedSV = std::basic_string_view<char_wrapper, std::char_traits<char_wrapper> >;
} using namespace libcis_ns_strings_string_view_string_view_comparison_common_type_specialization; // libcis


// std::common_type can be specialized and not have a typedef-name member type.
template <>
struct std::common_type<WrappedSV, WrappedSV> {};

namespace libcis_ns_strings_string_view_string_view_comparison_common_type_specialization { // libcis: isolate file-scope helpers
struct convertible_to_string_view {
  WrappedSV sv;
  convertible_to_string_view(const char_wrapper* a) : sv(a) {}
  operator WrappedSV() const { return sv; }
};

template <class T, class U>
void test() {
  char_wrapper a[] = {{'a'}, {'b'}, {'c'}, {'\0'}};

  assert((testComparisons(T(a), U(a), true, false)));

#if TEST_STD_VER > 17
  assert((testOrder(T(a), U(a), std::weak_ordering::equivalent)));
#endif
}
} using namespace libcis_ns_strings_string_view_string_view_comparison_common_type_specialization; // libcis


void test_strings_string_view_string_view_comparison_common_type_specialization() {
  test<WrappedSV, convertible_to_string_view>();

  return;
}
