// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_gcc_workaround_e78b1b3d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Tests workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=37804

#include <map>
namespace libcis_ns_containers_associative_map_gcc_workaround_e78b1b3d { // libcis
std::map<int, int>::iterator it;
#include <set>

#include "test_macros.h"
using std::multiset;
using std::set;
} // libcis_ns_containers_associative_map_gcc_workaround_e78b1b3d (libcis)

