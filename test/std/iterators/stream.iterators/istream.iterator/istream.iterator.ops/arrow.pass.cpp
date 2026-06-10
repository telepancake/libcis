// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istream_iterator_istream_iterator_ops_arrow).
// main -> test_iterators_stream_iterators_istream_iterator_istream_iterator_ops_arrow; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class istream_iterator

// const T* operator->() const;

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_iterators_stream_iterators_istream_iterator_istream_iterator_ops_arrow { // libcis: isolate file-scope helpers
struct A
{
    double d_;
    int i_;
};

void operator&(A const&) {}

std::istream& operator>>(std::istream& is, A& a)
{
    return is >> a.d_ >> a.i_;
}
} using namespace libcis_ns_iterators_stream_iterators_istream_iterator_istream_iterator_ops_arrow; // libcis


void test_iterators_stream_iterators_istream_iterator_istream_iterator_ops_arrow()
{
    std::istringstream inf("1.5  23 ");
    std::istream_iterator<A> i(inf);
    assert(i->d_ == 1.5);
    assert(i->i_ == 23);

  return;
}
