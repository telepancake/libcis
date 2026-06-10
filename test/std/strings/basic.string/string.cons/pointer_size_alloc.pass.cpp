// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_cons_pointer_size_alloc).
// main -> test_strings_basic_string_string_cons_pointer_size_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// basic_string(const charT* s, size_type n, const Allocator& a = Allocator()); // constexpr since C++20

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"

namespace libcis_ns_strings_basic_string_string_cons_pointer_size_alloc { // libcis: isolate file-scope helpers
template <class Alloc, class CharT>
TEST_CONSTEXPR_CXX20 void test(const CharT* s, unsigned n) {
  typedef std::basic_string<CharT, std::char_traits<CharT>, Alloc> S;
  typedef typename S::traits_type T;
  S s2(s, n);
  LIBCPP_ASSERT(s2.__invariants());
  assert(s2.size() == n);
  assert(T::compare(s2.data(), s, n) == 0);
  assert(s2.get_allocator() == Alloc());
  assert(s2.capacity() >= s2.size());
  LIBCPP_ASSERT(is_string_asan_correct(s2));
}

template <class Alloc, class CharT>
TEST_CONSTEXPR_CXX20 void test(const CharT* s, unsigned n, const Alloc& a) {
  typedef std::basic_string<CharT, std::char_traits<CharT>, Alloc> S;
  typedef typename S::traits_type T;
  S s2(s, n, a);
  LIBCPP_ASSERT(s2.__invariants());
  assert(s2.size() == n);
  assert(T::compare(s2.data(), s, n) == 0);
  assert(s2.get_allocator() == a);
  assert(s2.capacity() >= s2.size());
  LIBCPP_ASSERT(is_string_asan_correct(s2));
}

template <class Alloc>
TEST_CONSTEXPR_CXX20 void test(const Alloc& a) {
  test<Alloc>("", 0);
  test<Alloc>("", 0, Alloc(a));

  test<Alloc>("1", 1);
  test<Alloc>("1", 1, Alloc(a));

  test<Alloc>("1234567980", 10);
  test<Alloc>("1234567980", 10, Alloc(a));

  test<Alloc>("123456798012345679801234567980123456798012345679801234567980", 60);
  test<Alloc>("123456798012345679801234567980123456798012345679801234567980", 60, Alloc(a));
}

TEST_CONSTEXPR_CXX20 bool test() {
  test(std::allocator<char>());
  test(test_allocator<char>());
  test(test_allocator<char>(2));
#if TEST_STD_VER >= 11
  test(min_allocator<char>());
  test(safe_allocator<char>());
#endif

#if TEST_STD_VER >= 11
  { // LWG 2946
    std::string s({"abc", 1});
    assert(s.size() == 1);
    assert(s == "a");
  }
#endif

  return true;
}
} using namespace libcis_ns_strings_basic_string_string_cons_pointer_size_alloc; // libcis


void test_strings_basic_string_string_cons_pointer_size_alloc() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
