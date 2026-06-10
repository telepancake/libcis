// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_insert_iterator_cxx20_iter_member).
// main -> test_iterators_predef_iterators_insert_iterators_insert_iterator_cxx20_iter_member; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// insert_iterator
// C++20 and above use ranges::iterator_t<Container> instead of Container::iterator.

#include <iterator>

#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_iterators_predef_iterators_insert_iterators_insert_iterator_cxx20_iter_member { // libcis: isolate file-scope helpers
struct NoIteratorAlias {
    double data_[3] = {};
    double *begin();

    struct value_type {
        constexpr value_type(double d) : x(static_cast<int>(d)) {}
        constexpr operator double() const { return x; }

        int x;
    };

    template <class T>
    constexpr double *insert(double *pos, T value) {
        static_assert(std::is_same_v<T, value_type>);
        *pos = value;
        return pos;
    }
};

static_assert(std::is_constructible_v<std::insert_iterator<NoIteratorAlias>, NoIteratorAlias&, double*>);
static_assert(
    !std::is_constructible_v<std::insert_iterator<NoIteratorAlias>, NoIteratorAlias&, NoIteratorAlias::value_type*>);

constexpr bool test() {
    NoIteratorAlias c;
    double half = 0.5;
    auto it = std::insert_iterator<NoIteratorAlias>(c, c.data_);
    ASSERT_SAME_TYPE(decltype(std::inserter(c, c.data_)), std::insert_iterator<NoIteratorAlias>);
    *it++ = 1 + half;  // test that RHS is still implicitly converted to _Container::value_type
    *it++ = 2 + half;
    assert(c.data_[0] == 1.0);
    assert(c.data_[1] == 2.0);
    assert(c.data_[2] == 0.0);
    return true;
}
} using namespace libcis_ns_iterators_predef_iterators_insert_iterators_insert_iterator_cxx20_iter_member; // libcis


void test_iterators_predef_iterators_insert_iterators_insert_iterator_cxx20_iter_member() {
    test();
    static_assert(test());

    return;
}
