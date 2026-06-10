// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_size_max_size).
// main -> test_re_re_results_re_results_size_max_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// size_type max_size() const;

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_results_re_results_size_max_size { // libcis: isolate file-scope helpers
template <class CharT>
void
test()
{
    std::match_results<const CharT*> m;
    assert(m.max_size() > 0);
}
} using namespace libcis_ns_re_re_results_re_results_size_max_size; // libcis


void test_re_re_results_re_results_size_max_size()
{
    test<char>();
    test<wchar_t>();

  return;
}
