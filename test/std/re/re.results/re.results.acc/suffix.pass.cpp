// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_results_re_results_acc_suffix_24382300).
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

namespace libcis_ns_re_re_results_re_results_acc_suffix_24382300 { // libcis
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

int main(int, char**)
{
    test();

  return 0;

    return 0;
}
} // libcis_ns_re_re_results_re_results_acc_suffix_24382300 (libcis)

