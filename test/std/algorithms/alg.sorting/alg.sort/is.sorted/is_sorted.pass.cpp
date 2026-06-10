// AST-transferred from libc++ by tools/transfer.py (slug=algorithms_alg_sorting_alg_sort_is_sorted_is_sorted).
// main -> test_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<ForwardIterator Iter>
//   requires LessThanComparable<Iter::value_type>
//   bool
//   is_sorted(Iter first, Iter last);

#include <algorithm>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"

#if TEST_STD_VER > 17
namespace libcis_ns_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted { // libcis: isolate file-scope helpers
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {0, 0, 1, 1};
    int ib[] = {1, 1, 0, 0};
    return     std::is_sorted(std::begin(ia), std::end(ia))
           && !std::is_sorted(std::begin(ib), std::end(ib));
    }
#endif

template <class Iter>
void
test()
{
    {
    int a[] = {0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a)));
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }

    {
    int a[] = {0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }

    {
    int a[] = {0, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }

    {
    int a[] = {0, 0, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 0, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 0, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 0, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {0, 1, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 0, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(!std::is_sorted(Iter(a), Iter(a+sa)));
    }
    {
    int a[] = {1, 1, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    assert(std::is_sorted(Iter(a), Iter(a+sa)));
    }
}
} using namespace libcis_ns_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted; // libcis


void test_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted()
{
    test<forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*> >();
    test<const int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return;
}
