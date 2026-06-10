// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_bitset_members_none).
// main -> test_utilities_template_bitset_bitset_members_none; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// bool none() const; // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_bitset_members_none { // libcis: isolate file-scope helpers
template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_none() {
    std::bitset<N> v;
    v.reset();
    assert(v.none() == true);
    v.set();
    assert(v.none() == (N == 0));
    if (v.size() > 1) {
        v[N/2] = false;
        assert(v.none() == false);
        v.reset();
        v[N/2] = true;
        assert(v.none() == false);
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_none<0>();
  test_none<1>();
  test_none<31>();
  test_none<32>();
  test_none<33>();
  test_none<63>();
  test_none<64>();
  test_none<65>();
  test_none<1000>();

  return true;
}
} using namespace libcis_ns_utilities_template_bitset_bitset_members_none; // libcis


void test_utilities_template_bitset_bitset_members_none() {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return;
}
