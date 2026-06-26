// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_iterator_traits_empty_ae890c65).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// template<class NotAnIterator>
// struct iterator_traits
// {
// };

#include <iterator>

#include "test_macros.h"

namespace libcis_ns_iterators_iterator_primitives_iterator_traits_empty_ae890c65 { // libcis
struct not_an_iterator
{
};

template <class T>
struct has_value_type
{
private:
    struct two {char lx; char lxx;};
    template <class U> static two test(...);
    template <class U> static char test(typename U::value_type* = 0);
public:
    static const bool value = sizeof(test<T>(0)) == 1;
};

int main(int, char**)
{
    typedef std::iterator_traits<not_an_iterator> It;
    static_assert(!(has_value_type<It>::value), "");

    return 0;

    return 0;
}
} // libcis_ns_iterators_iterator_primitives_iterator_traits_empty_ae890c65 (libcis)

