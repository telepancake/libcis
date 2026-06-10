// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_const_default).
// main -> test_re_re_results_re_results_const_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// explicit match_results(const Allocator& a = Allocator()); // before C++20
// match_results() : match_results(Allocator()) {}           // C++20
// explicit match_results(const Allocator& a);               // C++20

#include <regex>
#include <cassert>
#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "test_convertible.h"

namespace libcis_ns_re_re_results_re_results_const_default { // libcis: isolate file-scope helpers
template <typename T>
void test_implicit() {
  static_assert(test_convertible<T>(), "");
  static_assert(!test_convertible<T, typename T::allocator_type>(), "");
}
#endif

template <class CharT>
void
test()
{
    typedef std::match_results<const CharT*> M;
    typedef std::allocator<std::sub_match<const CharT*> > Alloc;
    M m;
    assert(m.size() == 0);
    assert(!m.ready());
    assert(m.get_allocator() == Alloc());

#if TEST_STD_VER >= 11
    test_implicit<M>();
#endif
}
} using namespace libcis_ns_re_re_results_re_results_const_default; // libcis


void test_re_re_results_re_results_const_default()
{
    test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>();
#endif

  return;
}
