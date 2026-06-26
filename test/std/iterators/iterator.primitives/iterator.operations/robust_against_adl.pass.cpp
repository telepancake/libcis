// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_iterator_operations_robust_against_adl_a1a4f993).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

#include <iterator>

#include "test_macros.h"

namespace libcis_ns_iterators_iterator_primitives_iterator_operations_robust_against_adl_a1a4f993 { // libcis
struct Incomplete;
template<class T> struct Holder { T t; };

template<class>
struct Intable {
    operator int() const { return 1; }
};

int main(int, char**) {
    Holder<Incomplete> *a[2] = {};
    Holder<Incomplete> **p = a;
#if TEST_STD_VER >= 17
    p = std::next(p);
    p = std::prev(p);
    p = std::next(p, 2);
    p = std::prev(p, 2);
#endif
    std::advance(p, Intable<Holder<Incomplete> >());
    (void)std::distance(p, p);

    return 0;

    return 0;
}
} // libcis_ns_iterators_iterator_primitives_iterator_operations_robust_against_adl_a1a4f993 (libcis)

