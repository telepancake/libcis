// AST-transferred from libc++ by tools/transfer.py (slug=iterators_iterator_primitives_iterator_traits_volatile_pointer).
// main -> test_iterators_iterator_primitives_iterator_traits_volatile_pointer; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// template<class T>
// struct iterator_traits<const T*>

#include <iterator>
#include <cstddef>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_iterators_iterator_primitives_iterator_traits_volatile_pointer { // libcis: isolate file-scope helpers
struct A {};
} using namespace libcis_ns_iterators_iterator_primitives_iterator_traits_volatile_pointer; // libcis


void test_iterators_iterator_primitives_iterator_traits_volatile_pointer()
{
    typedef std::iterator_traits<volatile A*> It;
    static_assert((std::is_same<It::difference_type, std::ptrdiff_t>::value), "");
    static_assert((std::is_same<It::value_type, A>::value), "");
    static_assert((std::is_same<It::pointer, volatile A*>::value), "");
    static_assert((std::is_same<It::reference, volatile A&>::value), "");
    static_assert((std::is_same<It::iterator_category, std::random_access_iterator_tag>::value), "");
#if TEST_STD_VER > 17
    ASSERT_SAME_TYPE(It::iterator_concept, std::contiguous_iterator_tag);
#endif
    return;
}
