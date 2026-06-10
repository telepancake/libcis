// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_adapt_rand_adapt_shuf_io).
// main -> test_numerics_rand_rand_adapt_rand_adapt_shuf_io; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization

// <random>

// template<class Engine, size_t k>
// class shuffle_order_engine

// template <class charT, class traits,
//           class Engine, size_t k>
// basic_ostream<charT, traits>&
// operator<<(basic_ostream<charT, traits>& os,
//            const shuffle_order_engine<Engine, k>& x);
//
// template <class charT, class traits,
//           class Engine, size_t k>
// basic_istream<charT, traits>&
// operator>>(basic_istream<charT, traits>& is,
//            shuffle_order_engine<Engine, k>& x);

#include <random>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_io { // libcis: isolate file-scope helpers
void
test1()
{
    typedef std::knuth_b E;
    E e1;
    e1.discard(100);
    std::ostringstream os;
    os << e1;
    std::istringstream is(os.str());
    E e2;
    is >> e2;
    assert(e1 == e2);
}
} using namespace libcis_ns_numerics_rand_rand_adapt_rand_adapt_shuf_io; // libcis


void test_numerics_rand_rand_adapt_rand_adapt_shuf_io()
{
    test1();

  return;
}
