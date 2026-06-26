// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_reverse_iterator_692681b9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// reverse_iterator

// template <class U>
// reverse_iterator(const reverse_iterator<U> &u); // constexpr since C++17

#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_reverse_iterator_692681b9 { // libcis
struct Base { };
struct Derived : Base { };

template <class It, class U>
TEST_CONSTEXPR_CXX17 void test(U u) {
    const std::reverse_iterator<U> r2(u);
    std::reverse_iterator<It> r1 = r2;
    assert(base(r1.base()) == base(u));
}

TEST_CONSTEXPR_CXX17 bool tests() {
    Derived d;
    test<bidirectional_iterator<Base*> >(bidirectional_iterator<Derived*>(&d));
    test<random_access_iterator<const Base*> >(random_access_iterator<Derived*>(&d));
#if TEST_STD_VER >= 20
    test<cpp20_random_access_iterator<const Base*> >(cpp20_random_access_iterator<Derived*>(&d));
#endif
    test<Base*>(&d);
    return true;
}

int main(int, char**) {
    tests();
#if TEST_STD_VER > 14
    static_assert(tests(), "");
#endif
    return 0;

    return 0;
}
} // libcis_ns_iterators_predef_iterators_reverse_iterators_reverse_iter_cons_ctor_reverse_iterator_692681b9 (libcis)

