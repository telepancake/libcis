// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pairs_pair_pair_incomplete).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Check that instantiating pair doesn't look up type traits "too early", before
// the contained types have been completed.
//
// This is a regression test, to prevent a reoccurrence of the issue introduced
// in 5e1de27f680591a870d78e9952b23f76aed7f456.

#include <utility>
#include <vector>

namespace libcis_ns_utilities_utility_pairs_pairs_pair_pair_incomplete { // libcis: isolate file-scope helpers
struct Test {
  std::vector<std::pair<int, Test> > v;
};

std::pair<int, Test> p;
} using namespace libcis_ns_utilities_utility_pairs_pairs_pair_pair_incomplete; // libcis

