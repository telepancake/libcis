// transferred+adapted from libc++ by tools/transfer.py (slug=algorithms_alg_sorting_alg_min_max_min_init_list_comp_f84ddb9a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <algorithm>

// template<class T, class Compare>
//   T
//   min(initializer_list<T> t, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_algorithms_alg_sorting_alg_min_max_min_init_list_comp_f84ddb9a { // libcis
int main(int, char**)
{
    int i = std::min({2, 3, 1}, std::greater<int>());
    assert(i == 3);
    i = std::min({2, 1, 3}, std::greater<int>());
    assert(i == 3);
    i = std::min({3, 1, 2}, std::greater<int>());
    assert(i == 3);
    i = std::min({3, 2, 1}, std::greater<int>());
    assert(i == 3);
    i = std::min({1, 2, 3}, std::greater<int>());
    assert(i == 3);
    i = std::min({1, 3, 2}, std::greater<int>());
    assert(i == 3);
#if TEST_STD_VER >= 14
    {
    static_assert(std::min({1, 3, 2}, std::greater<int>()) == 3, "");
    static_assert(std::min({2, 1, 3}, std::greater<int>()) == 3, "");
    static_assert(std::min({3, 2, 1}, std::greater<int>()) == 3, "");
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_algorithms_alg_sorting_alg_min_max_min_init_list_comp_f84ddb9a (libcis)

