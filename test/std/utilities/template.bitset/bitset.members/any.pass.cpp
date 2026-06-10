// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_bitset_members_any).
// main -> test_utilities_template_bitset_bitset_members_any; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// bool any() const; // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_bitset_members_any { // libcis: isolate file-scope helpers
template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_any() {
    std::bitset<N> v;
    v.reset();
    assert(v.any() == false);
    v.set();
    assert(v.any() == (N != 0));
    if (v.size() > 1) {
        v[N/2] = false;
        assert(v.any() == true);
        v.reset();
        v[N/2] = true;
        assert(v.any() == true);
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_any<0>();
  test_any<1>();
  test_any<31>();
  test_any<32>();
  test_any<33>();
  test_any<63>();
  test_any<64>();
  test_any<65>();
  test_any<1000>();

  return true;
}
} using namespace libcis_ns_utilities_template_bitset_bitset_members_any; // libcis


void test_utilities_template_bitset_bitset_members_any() {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return;
}
