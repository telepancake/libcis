// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_const_move_assign).
// main -> test_re_re_results_re_results_const_move_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// match_results& operator=(match_results&& m);

#include <regex>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_re_re_results_re_results_const_move_assign { // libcis: isolate file-scope helpers
template <class CharT, class Allocator>
void
test(const Allocator& a)
{
    typedef std::match_results<const CharT*, Allocator> SM;
    SM m0(a);
    SM m1;

    m1 = std::move(m0);
    assert(m1.size()          == 0);
    assert(!m1.ready());
    if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value)
        assert(m1.get_allocator() == a);
    else
        assert(m1.get_allocator() == Allocator());
}
} using namespace libcis_ns_re_re_results_re_results_const_move_assign; // libcis


void test_re_re_results_re_results_const_move_assign()
{
    test<char>   (std::allocator<std::sub_match<const char *> >());
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>(std::allocator<std::sub_match<const wchar_t *> >());
#endif

    // test_allocator has POCMA -> false
    test<char>   (test_allocator<std::sub_match<const char*> >(3));
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>(test_allocator<std::sub_match<const wchar_t*> >(3));
#endif

    // other_allocator has POCMA -> true
    test<char>   (other_allocator<std::sub_match<const char*> >(3));
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>(other_allocator<std::sub_match<const wchar_t*> >(3));
#endif

  return;
}
