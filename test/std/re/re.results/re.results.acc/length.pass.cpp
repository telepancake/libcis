// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_acc_length).
// main -> test_re_re_results_re_results_acc_length; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// difference_type length(size_type sub = 0) const;

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_results_re_results_acc_length { // libcis: isolate file-scope helpers
void
test()
{
    std::match_results<const char*> m;
    const char s[] = "abcdefghijk";
    assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));
    assert(m.length() == m[0].length());
    assert(m.length(0) == m[0].length());
    assert(m.length(1) == m[1].length());
    assert(m.length(2) == m[2].length());
    assert(m.length(3) == m[3].length());
    assert(m.length(4) == m[4].length());
}
} using namespace libcis_ns_re_re_results_re_results_acc_length; // libcis


void test_re_re_results_re_results_acc_length()
{
    test();

  return;
}
