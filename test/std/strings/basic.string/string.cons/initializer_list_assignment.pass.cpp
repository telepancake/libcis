// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_cons_initializer_list_assignment).
// main -> test_strings_basic_string_string_cons_initializer_list_assignment; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <string>

// basic_string& operator=(initializer_list<charT> il); // constexpr since C++20

#include <string>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

// clang-format off
namespace libcis_ns_strings_basic_string_string_cons_initializer_list_assignment { // libcis: isolate file-scope helpers
template <template <class> class Alloc>
TEST_CONSTEXPR_CXX20 void test_string() {
  {
    typedef std::basic_string<char, std::char_traits<char>, Alloc<char>> S;
    S s;
    S& result = (s = {'a', 'b', 'c'});
    assert(s == "abc");
    assert(&result == &s);
    LIBCPP_ASSERT(is_string_asan_correct(s));
    LIBCPP_ASSERT(is_string_asan_correct(result));
  }
  {
    typedef std::basic_string<char, std::char_traits<char>, Alloc<char>> S;
    S s;
    s = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
         'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
    assert(s == "aaaaaaaaaaaaaaaaaaaaaaaa");
    LIBCPP_ASSERT(is_string_asan_correct(s));
  }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  {
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t>> S;
    S s;
    S& result = (s = {L'a', L'b', L'c'});
    assert(s == L"abc");
    assert(&result == &s);
    LIBCPP_ASSERT(is_string_asan_correct(s));
    LIBCPP_ASSERT(is_string_asan_correct(result));
  }
  {
    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t>> S;
    S s;
    S& result = (s = {L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a', L'a'});
    assert(s == L"aaaaaaaaaaaaaaaaaaaaaaaaa");
    assert(&result == &s);
    LIBCPP_ASSERT(is_string_asan_correct(s));
    LIBCPP_ASSERT(is_string_asan_correct(result));
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
} using namespace libcis_ns_strings_basic_string_string_cons_initializer_list_assignment; // libcis


void test_strings_basic_string_string_cons_initializer_list_assignment() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
