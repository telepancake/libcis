// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_bitset_members_flip_one).
// main -> test_utilities_template_bitset_bitset_members_flip_one; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// bitset<N>& flip(size_t pos); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_bitset_members_flip_one { // libcis: isolate file-scope helpers
template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_flip_one() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c = 0; c != cases.size(); ++c) {
        std::bitset<N> v = cases[c];
        if (v.size() > 0) {
            std::size_t middle = v.size() / 2;
            v.flip(middle);
            bool b = v[middle];
            assert(v[middle] == b);
            v.flip(middle);
            assert(v[middle] != b);
            v.flip(middle);
            assert(v[middle] == b);
        }
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_flip_one<0>();
  test_flip_one<1>();
  test_flip_one<31>();
  test_flip_one<32>();
  test_flip_one<33>();
  test_flip_one<63>();
  test_flip_one<64>();
  test_flip_one<65>();

  return true;
}
} using namespace libcis_ns_utilities_template_bitset_bitset_members_flip_one; // libcis


void test_utilities_template_bitset_bitset_members_flip_one() {
  test();
  test_flip_one<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return;
}
