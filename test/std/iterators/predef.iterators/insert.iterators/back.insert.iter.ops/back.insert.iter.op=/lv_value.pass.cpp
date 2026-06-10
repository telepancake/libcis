// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_insert_iterators_back_insert_iter_ops_back_insert_iter_op_lv_value).
// main -> test_iterators_predef_iterators_insert_iterators_back_insert_iter_ops_back_insert_iter_op_lv_value; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// back_insert_iterator

// requires CopyConstructible<Cont::value_type>
//   back_insert_iterator<Cont>&
//   operator=(const Cont::value_type& value);

#include <iterator>
#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_constexpr_container.h"

namespace libcis_ns_iterators_predef_iterators_insert_iterators_back_insert_iter_ops_back_insert_iter_op_lv_value { // libcis: isolate file-scope helpers
template <class C>
TEST_CONSTEXPR_CXX14 bool
test(C c)
{
    const typename C::value_type v = typename C::value_type();
    std::back_insert_iterator<C> i(c);
    i = v;
    assert(c.back() == v);
    return true;
}

class Copyable
{
    int data_;
public:
    Copyable() : data_(0) {}
    Copyable(const Copyable&) = default;
    Copyable& operator=(const Copyable&) = default;
    ~Copyable() {data_ = -1;}

    friend bool operator==(const Copyable& x, const Copyable& y)
        {return x.data_ == y.data_;}
};
} using namespace libcis_ns_iterators_predef_iterators_insert_iterators_back_insert_iter_ops_back_insert_iter_op_lv_value; // libcis


void test_iterators_predef_iterators_insert_iterators_back_insert_iter_ops_back_insert_iter_op_lv_value()
{
    test(std::vector<Copyable>());
#if TEST_STD_VER >= 20
    test(ConstexprFixedCapacityDeque<int, 10>());
    static_assert(test(ConstexprFixedCapacityDeque<int, 10>()));
#endif
    return;
}
