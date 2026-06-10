// AST-transferred from libc++ by tools/transfer.py (slug=utilities_format_format_arguments_format_arg_operator_bool).
// main -> test_utilities_format_format_arguments_format_arg_operator_bool; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <format>

// explicit operator bool() const noexcept
//
// Note more testing is done in the unit test for:
// template<class Visitor, class Context>
//   see below visit_format_arg(Visitor&& vis, basic_format_arg<Context> arg);

#include <format>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_format_format_arguments_format_arg_operator_bool { // libcis: isolate file-scope helpers
template <class CharT>
void test() {
  using Context = std::basic_format_context<CharT*, CharT>;
  {
    std::basic_format_arg<Context> format_arg{};
    ASSERT_NOEXCEPT(!format_arg);
    assert(!format_arg);
    ASSERT_NOEXCEPT(static_cast<bool>(format_arg));
    assert(!static_cast<bool>(format_arg));
  }
}
} using namespace libcis_ns_utilities_format_format_arguments_format_arg_operator_bool; // libcis


void test_utilities_format_format_arguments_format_arg_operator_bool() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return;
}
