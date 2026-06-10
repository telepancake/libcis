// AST-transferred from libc++ by tools/transfer.py (slug=algorithms_alg_sorting_alg_min_max_max).
// main -> test_algorithms_alg_sorting_alg_min_max_max; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<LessThanComparable T>
//   const T&
//   max(const T& a, const T& b);

#include <algorithm>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_algorithms_alg_sorting_alg_min_max_max { // libcis: isolate file-scope helpers
template <class T>
void
test(const T& a, const T& b, const T& x)
{
    assert(&std::max(a, b) == &x);
}
} using namespace libcis_ns_algorithms_alg_sorting_alg_min_max_max; // libcis


void test_algorithms_alg_sorting_alg_min_max_max()
{
    {
    int x = 0;
    int y = 0;
    test(x, y, x);
    test(y, x, y);
    }
    {
    int x = 0;
    int y = 1;
    test(x, y, y);
    test(y, x, y);
    }
    {
    int x = 1;
    int y = 0;
    test(x, y, x);
    test(y, x, x);
    }
#if TEST_STD_VER >= 14
    {
    constexpr int x = 1;
    constexpr int y = 0;
    static_assert(std::max(x, y) == x, "" );
    static_assert(std::max(y, x) == x, "" );
    }
#endif

  return;
}
