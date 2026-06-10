// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_cons_initializer_list).
// main -> test_strings_basic_string_string_cons_initializer_list; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <string>

// basic_string(initializer_list<charT> il, const Allocator& a = Allocator()); // constexpr since C++20

#include <string>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"

// clang-format off
namespace libcis_ns_strings_basic_string_string_cons_initializer_list { // libcis: isolate file-scope helpers
template <template <class> class Alloc>
TEST_CONSTEXPR_CXX20 void test_string() {
  {
    std::basic_string<char, std::char_traits<char>, Alloc<char> > s = {'a', 'b', 'c'};
    assert(s == "abc");
    LIBCPP_ASSERT(is_string_asan_correct(s));
  }
  {
    std::basic_string<char, std::char_traits<char>, Alloc<char> > s = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
    assert(s == "aaaaaaaaaaaaaaaaaaaaaaaaa");
    LIBCPP_ASSERT(is_string_asan_correct(s));
  }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  {
    std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t> > s = {L'a', L'b', L'c'};
    assert(s == L"abc");
    LIBCPP_ASSERT(is_string_asan_correct(s));
  }
  {
    std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t> > s = {L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a'};
    assert(s == L"aaaaaaaaaaaaaaaaaaaaaaaaa");
    LIBCPP_ASSERT(is_string_asan_correct(s));
  }
#endif
}
// clang-format on

TEST_CONSTEXPR_CXX20 bool test() {
  test_string<std::allocator>();
  test_string<min_allocator>();
  test_string<safe_allocator>();

  return true;
}
} using namespace libcis_ns_strings_basic_string_string_cons_initializer_list; // libcis


void test_strings_basic_string_string_cons_initializer_list() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
