// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_gcc_workaround_99a6e76c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Tests workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=37804

#include <map>
#include <set> // libcis: hoisted out of namespace (mid-file include cannot appear inside the test namespace); map-then-set include order preserved

#include "test_macros.h"

namespace libcis_ns_containers_associative_map_gcc_workaround_99a6e76c { // libcis
std::map<int,int>::iterator it;
using std::set;
using std::multiset;

int main(int, char**) {
    return 0;
}
} // libcis_ns_containers_associative_map_gcc_workaround_99a6e76c (libcis)
