// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_acc_suffix).
// main -> test_re_re_results_re_results_acc_suffix; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// const_reference suffix() const;

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_results_re_results_acc_suffix { // libcis: isolate file-scope helpers
void
test()
{
    std::match_results<const char*> m;
    const char s[] = "abcdefghijk";
    assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));

    assert(m.suffix().first == s+9);
    assert(m.suffix().second == s+11);
    assert(m.suffix().matched == true);
}
} using namespace libcis_ns_re_re_results_re_results_acc_suffix; // libcis


void test_re_re_results_re_results_acc_suffix()
{
    test();

  return;
}
