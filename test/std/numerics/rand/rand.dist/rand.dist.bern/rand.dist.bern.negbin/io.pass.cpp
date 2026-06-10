// AST-transferred from libc++ by tools/transfer.py (slug=numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_io).
// main -> test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_io; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization

// <random>

// template<class IntType = int>
// class negative_binomial_distribution

// template <class charT, class traits>
// basic_ostream<charT, traits>&
// operator<<(basic_ostream<charT, traits>& os,
//            const negative_binomial_distribution& x);
//
// template <class charT, class traits>
// basic_istream<charT, traits>&
// operator>>(basic_istream<charT, traits>& is,
//            negative_binomial_distribution& x);

#include <random>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_numerics_rand_rand_dist_rand_dist_bern_rand_dist_bern_negbin_io()
{
    {
        typedef std::negative_binomial_distribution<> D;
        D d1(7, .25);
        std::ostringstream os;
        os << d1;
        std::istringstream is(os.str());
        D d2;
        is >> d2;
        assert(d1 == d2);
    }

  return;
}
