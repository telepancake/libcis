// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_const_allocator).
// main -> test_re_re_results_re_results_const_allocator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// match_results(const Allocator& a = Allocator());

#include <regex>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_re_re_results_re_results_const_allocator { // libcis: isolate file-scope helpers
template <class CharT, class Allocator>
void
test(const Allocator& a)
{
    std::match_results<const CharT*, Allocator> m(a);
    assert(m.size() == 0);
    assert(!m.ready());
    assert(m.get_allocator() == a);
}
} using namespace libcis_ns_re_re_results_re_results_const_allocator; // libcis


void test_re_re_results_re_results_const_allocator()
{
    test<char>(test_allocator<std::sub_match<const char*> >(3));
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>(test_allocator<std::sub_match<const wchar_t*> >(3));
#endif

  return;
}
