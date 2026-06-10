// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_bitset_members_set_all).
// main -> test_utilities_template_bitset_bitset_members_set_all; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// bitset<N>& set(); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_bitset_members_set_all { // libcis: isolate file-scope helpers
template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_set_all() {
    std::bitset<N> v;
    v.set();
    for (std::size_t i = 0; i < v.size(); ++i)
        assert(v[i]);
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_set_all<0>();
  test_set_all<1>();
  test_set_all<31>();
  test_set_all<32>();
  test_set_all<33>();
  test_set_all<63>();
  test_set_all<64>();
  test_set_all<65>();
  test_set_all<1000>();

  return true;
}
} using namespace libcis_ns_utilities_template_bitset_bitset_members_set_all; // libcis


void test_utilities_template_bitset_bitset_members_set_all() {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return;
}
