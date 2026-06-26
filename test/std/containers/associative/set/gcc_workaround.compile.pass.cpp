// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_set_gcc_workaround_72587629).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Tests workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=37804

#include <set>
namespace libcis_ns_containers_associative_set_gcc_workaround_72587629 { // libcis
std::set<int> s;
#include <map>

#include "test_macros.h"
using std::map;
using std::multimap;
} // libcis_ns_containers_associative_set_gcc_workaround_72587629 (libcis)

