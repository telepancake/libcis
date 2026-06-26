// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterators_general_gcc_workaround_de2fb9fc).
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

namespace libcis_ns_iterators_iterators_general_gcc_workaround_de2fb9fc { // libcis
void f(const std::string &s) { TEST_IGNORE_NODISCARD s.begin(); }

#include <vector>

void AppendTo(const std::vector<char> &v) { TEST_IGNORE_NODISCARD v.begin(); }

int main(int, char**) {
  return 0;

    return 0;
}
} // libcis_ns_iterators_iterators_general_gcc_workaround_de2fb9fc (libcis)

