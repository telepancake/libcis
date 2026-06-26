// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_difference_type_f4b10f9c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// move_iterator

// requires RandomAccessIterator<Iter>
//   move_iterator& operator-=(difference_type n);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_difference_type_f4b10f9c { // libcis
template <class It>
void
test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    std::move_iterator<It> r(i);
    std::move_iterator<It>& rr = r -= n;
    assert(r.base() == x);
    assert(&rr == &r);
}

int main(int, char**)
{
    const char* s = "1234567890";
    test(random_access_iterator<const char*>(s+5), 5, random_access_iterator<const char*>(s));
    test(s+5, 5, s);

#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    constexpr auto it1 = std::make_move_iterator(p);
    constexpr auto it2 = std::make_move_iterator(p+5) -= 5;
    static_assert(it1 == it2, "");
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_iterators_predef_iterators_move_iterators_move_iter_ops_move_iter_op_difference_type_f4b10f9c (libcis)

