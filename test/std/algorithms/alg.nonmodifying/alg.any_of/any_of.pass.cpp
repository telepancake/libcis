// AST-transferred from libc++ by tools/transfer.py (slug=algorithms_alg_nonmodifying_alg_any_of_any_of).
// main -> test_algorithms_alg_nonmodifying_alg_any_of_any_of; file-scope helpers isolated in anon namespace.
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
//   any_of(InputIterator first, InputIterator last, Predicate pred);

#include <algorithm>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_algorithms_alg_nonmodifying_alg_any_of_any_of { // libcis: isolate file-scope helpers
struct test1
{
    TEST_CONSTEXPR bool operator()(const int& i) const
    {
        return i % 2 == 0;
    }
};

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {2, 4, 6, 8};
    int ib[] = {1, 3, 5, 7};
    return  std::any_of(std::begin(ia), std::end(ia), test1())
        && !std::any_of(std::begin(ib), std::end(ib), test1())
        ;
    }
} using namespace libcis_ns_algorithms_alg_nonmodifying_alg_any_of_any_of; // libcis

#endif

void test_algorithms_alg_nonmodifying_alg_any_of_any_of()
{
    {
        int ia[] = {2, 4, 6, 8};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        assert(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia + sa), test1()) == true);
        assert(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia), test1()) == false);
    }
    {
        const int ia[] = {2, 4, 5, 8};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        assert(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia + sa), test1()) == true);
        assert(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia), test1()) == false);
    }
    {
        const int ia[] = {1, 3, 5, 7};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        assert(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia + sa), test1()) == false);
        assert(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia), test1()) == false);
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return;
}
