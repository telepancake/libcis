// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_includes).
// main -> test_utilities_template_bitset_includes; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test that <bitset> includes <string> and <iosfwd>

#include <bitset>

#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_includes { // libcis: isolate file-scope helpers
template <class> void test_typedef() {}
} using namespace libcis_ns_utilities_template_bitset_includes; // libcis


void test_utilities_template_bitset_includes()
{
  { // test for <string>
    std::string s; ((void)s);
  }
  { // test for <iosfwd>
    test_typedef<std::ios>();
    test_typedef<std::istream>();
    test_typedef<std::ostream>();
    test_typedef<std::iostream>();
  }

  return;
}
