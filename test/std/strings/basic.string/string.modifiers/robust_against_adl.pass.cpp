// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_modifiers_robust_against_adl).
// main -> test_strings_basic_string_string_modifiers_robust_against_adl; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

#include <cassert>
#include <string>

#include "test_macros.h"

namespace libcis_ns_strings_basic_string_string_modifiers_robust_against_adl { // libcis: isolate file-scope helpers
struct Incomplete;
template <class T>
struct Holder {
  T t;
};

template <class T>
struct Charlike {
  char ch_;
  TEST_CONSTEXPR Charlike(char ch) : ch_(ch) {}
  TEST_CONSTEXPR operator char() const { return ch_; }
};

template <class S>
TEST_CONSTEXPR_CXX20 void test_string() {
  S s;
  Charlike<Holder<Incomplete> > a[] = {'m', 'a', 'h', 'i'};
  s.append(a, a + 4);
  s.assign(a, a + 4);
  s.insert(s.begin(), a, a + 4);
  s.replace(s.begin(), s.begin() + 4, a, a + 4);
  assert(s == "mahimahi");
}

TEST_CONSTEXPR_CXX20 bool test() {
  test_string<std::string>();

  return true;
}
} using namespace libcis_ns_strings_basic_string_string_modifiers_robust_against_adl; // libcis


void test_strings_basic_string_string_modifiers_robust_against_adl() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
