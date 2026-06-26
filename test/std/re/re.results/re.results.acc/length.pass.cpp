// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_results_re_results_acc_length_3965fe0a).
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

namespace libcis_ns_re_re_results_re_results_acc_length_3965fe0a { // libcis
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

int main(int, char**)
{
    test();

  return 0;

    return 0;
}
} // libcis_ns_re_re_results_re_results_acc_length_3965fe0a (libcis)

