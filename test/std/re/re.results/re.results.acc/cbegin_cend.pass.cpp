// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_acc_cbegin_cend).
// main -> test_re_re_results_re_results_acc_cbegin_cend; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// const_iterator cbegin() const;
// const_iterator cend() const;

#include <regex>
#include <cassert>
#include <cstddef>
#include "test_macros.h"

namespace libcis_ns_re_re_results_re_results_acc_cbegin_cend { // libcis: isolate file-scope helpers
void
test()
{
    std::match_results<const char*> m;
    const char s[] = "abcdefghijk";
    assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));

    std::match_results<const char*>::const_iterator i = m.cbegin();
    std::match_results<const char*>::const_iterator e = m.cend();

    assert(static_cast<std::size_t>(e - i) == m.size());
    for (int j = 0; i != e; ++i, ++j)
        assert(*i == m[j]);
}
} using namespace libcis_ns_re_re_results_re_results_acc_cbegin_cend; // libcis


void test_re_re_results_re_results_acc_cbegin_cend()
{
    test();

  return;
}
