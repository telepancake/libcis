// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_map_gcc_workaround).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Tests workaround for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=37804

#include <map>
namespace libcis_ns_containers_associative_map_gcc_workaround { // libcis: isolate file-scope helpers
std::map<int, int>::iterator it;
} using namespace libcis_ns_containers_associative_map_gcc_workaround; // libcis

#include <set>

#include "test_macros.h"
using std::multiset;
using std::set;
