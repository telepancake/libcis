// AST-transferred from libc++ by tools/transfer.py (slug=re_re_results_re_results_acc_str).
// main -> test_re_re_results_re_results_acc_str; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// string_type str(size_type sub = 0) const;

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_results_re_results_acc_str { // libcis: isolate file-scope helpers
void
test()
{
    std::match_results<const char*> m;
    const char s[] = "abcdefghijk";
    assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));
    assert(m.str() == std::string(m[0]));
    assert(m.str(0) == std::string(m[0]));
    assert(m.str(1) == std::string(m[1]));
    assert(m.str(2) == std::string(m[2]));
    assert(m.str(3) == std::string(m[3]));
    assert(m.str(4) == std::string(m[4]));
}
} using namespace libcis_ns_re_re_results_re_results_acc_str; // libcis


void test_re_re_results_re_results_acc_str()
{
    test();

  return;
}
