// AST-transferred from libc++ by tools/transfer.py (slug=algorithms_alg_modifying_operations_alg_partitions_is_partitioned).
// main -> test_algorithms_alg_modifying_operations_alg_partitions_is_partitioned; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template <class InputIterator, class Predicate>
//     constexpr bool       // constexpr after C++17
//     is_partitioned(InputIterator first, InputIterator last, Predicate pred);

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"
#include "counting_predicates.h"

namespace libcis_ns_algorithms_alg_modifying_operations_alg_partitions_is_partitioned { // libcis: isolate file-scope helpers
struct is_odd {
  TEST_CONSTEXPR bool operator()(const int &i) const { return i & 1; }
};

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 4, 6};
    int ib[] = {1, 2, 3, 4, 5, 6};
    return     std::is_partitioned(std::begin(ia), std::end(ia), is_odd())
           && !std::is_partitioned(std::begin(ib), std::end(ib), is_odd());
    }
} using namespace libcis_ns_algorithms_alg_modifying_operations_alg_partitions_is_partitioned; // libcis

#endif


void test_algorithms_alg_modifying_operations_alg_partitions_is_partitioned() {
  {
    const int ia[] = {1, 2, 3, 4, 5, 6};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    assert(!std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                                cpp17_input_iterator<const int *>(std::end(ia)),
                                std::ref(pred)));
    assert(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 2, 4, 6};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    assert(std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                               cpp17_input_iterator<const int *>(std::end(ia)),
                               std::ref(pred)));
    assert(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {2, 4, 6, 1, 3, 5};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    assert(!std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                                cpp17_input_iterator<const int *>(std::end(ia)),
                                std::ref(pred)));
    assert(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 2, 4, 6, 7};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    assert(!std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                                cpp17_input_iterator<const int *>(std::end(ia)),
                                std::ref(pred)));
    assert(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 2, 4, 6, 7};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    assert(std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                               cpp17_input_iterator<const int *>(std::begin(ia)),
                               std::ref(pred)));
    assert(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::begin(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 7, 9, 11, 2};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    assert(std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                               cpp17_input_iterator<const int *>(std::end(ia)),
                               std::ref(pred)));
    assert(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return;
}
