// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_set_gcc_workaround_4806d853).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Tests workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=37804

#include <set>
#include <map> // libcis: hoisted out of namespace (mid-file include cannot appear inside the test namespace); set-then-map include order preserved

#include "test_macros.h"

namespace libcis_ns_containers_associative_set_gcc_workaround_4806d853 { // libcis
std::set<int> s;
using std::map;
using std::multimap;

int main(int, char**) {
    return 0;
}
} // libcis_ns_containers_associative_set_gcc_workaround_4806d853 (libcis)
