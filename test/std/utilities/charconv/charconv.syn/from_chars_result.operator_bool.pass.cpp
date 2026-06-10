// AST-transferred from libc++ by tools/transfer.py (slug=utilities_charconv_charconv_syn_from_chars_result_operator_bool).
// main -> test_utilities_charconv_charconv_syn_from_chars_result_operator_bool; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20, c++23

// <charconv>

// struct from_chars_result
//   constexpr explicit operator bool() const noexcept { return ec == errc{}; }

#include <charconv>

#include <cassert>
#include <system_error>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_charconv_charconv_syn_from_chars_result_operator_bool { // libcis: isolate file-scope helpers
static_assert(!std::is_convertible_v<std::from_chars_result, bool>);
static_assert(std::is_constructible_v<bool, std::from_chars_result>);

constexpr bool test() {
  // True
  {
    std::from_chars_result value{nullptr, std::errc{}};
    assert(bool(value) == true);
    static_assert(noexcept(bool(value)) == true);
  }
  // False
  {
    std::from_chars_result value{nullptr, std::errc::value_too_large};
    assert(bool(value) == false);
    static_assert(noexcept(bool(value)) == true);
  }

  return true;
}
} using namespace libcis_ns_utilities_charconv_charconv_syn_from_chars_result_operator_bool; // libcis


void test_utilities_charconv_charconv_syn_from_chars_result_operator_bool() {
  assert(test());
  static_assert(test());

  return;
}
