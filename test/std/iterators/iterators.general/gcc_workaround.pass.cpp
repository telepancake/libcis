// AST-transferred from libc++ by tools/transfer.py (slug=iterators_iterators_general_gcc_workaround).
// main -> test_iterators_iterators_general_gcc_workaround; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Tests workaround for  https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64816.

#include <string>
#include "test_macros.h"

namespace libcis_ns_iterators_iterators_general_gcc_workaround { // libcis: isolate file-scope helpers
void f(const std::string &s) { TEST_IGNORE_NODISCARD s.begin(); }

#include <vector>

void AppendTo(const std::vector<char> &v) { TEST_IGNORE_NODISCARD v.begin(); }
} using namespace libcis_ns_iterators_iterators_general_gcc_workaround; // libcis


void test_iterators_iterators_general_gcc_workaround() {
  return;
}
