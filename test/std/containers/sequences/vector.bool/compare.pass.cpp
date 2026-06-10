// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_compare).
// main -> test_containers_sequences_vector_bool_compare; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// bool operator==( const vector& lhs, const vector& rhs );
// bool operator!=( const vector& lhs, const vector& rhs );
// bool operator< ( const vector& lhs, const vector& rhs );
// bool operator<=( const vector& lhs, const vector& rhs );
// bool operator> ( const vector& lhs, const vector& rhs );
// bool operator>=( const vector& lhs, const vector& rhs );

#include <vector>
#include <cassert>

#include "test_comparisons.h"

namespace libcis_ns_containers_sequences_vector_bool_compare { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool test() {
  typedef std::vector<bool> VB;
  {
    const VB v1, v2;
    assert(testComparisons(v1, v2, true, false));
  }
  {
    const VB v1(1, true);
    const VB v2(1, true);
    assert(testComparisons(v1, v2, true, false));
  }
  {
    const VB v1(1, false);
    const VB v2(1, true);
    assert(testComparisons(v1, v2, false, true));
  }
  {
    const VB v1, v2(1, true);
    assert(testComparisons(v1, v2, false, true));
  }
  {
    bool items1[3] = {false, true, false};
    bool items2[3] = {false, true, true};
    const VB v1(items1, items1 + 3);
    const VB v2(items2, items2 + 3);
    assert(testComparisons(v1, v2, false, true));
  }
  {
    bool items1[3] = {false, false, false};
    bool items2[3] = {false, true, false};
    const VB v1(items1, items1 + 3);
    const VB v2(items2, items2 + 3);
    assert(testComparisons(v1, v2, false, true));
  }
  {
    bool items1[2] = {false, true};
    bool items2[3] = {false, true, false};
    const VB v1(items1, items1 + 2);
    const VB v2(items2, items2 + 3);
    assert(testComparisons(v1, v2, false, true));
  }
  {
    bool items[3] = {false, true, false};
    const VB v1(items, items + 3);
    const VB v2(1, true);
    assert(testComparisons(v1, v2, false, true));
  }
  {
    assert((std::vector<bool>() == std::vector<bool>()));
    assert(!(std::vector<bool>() != std::vector<bool>()));
    assert(!(std::vector<bool>() < std::vector<bool>()));
    assert((std::vector<bool>() <= std::vector<bool>()));
    assert(!(std::vector<bool>() > std::vector<bool>()));
    assert((std::vector<bool>() >= std::vector<bool>()));
  }

  return true;
}
} using namespace libcis_ns_containers_sequences_vector_bool_compare; // libcis


void test_containers_sequences_vector_bool_compare() {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return;
}
