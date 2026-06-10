// AST-transferred from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_alg_req_ind_move_indirectly_movable_subsumption).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class In, class Out>
// concept indirectly_movable;

#include <iterator>

namespace libcis_ns_iterators_iterator_requirements_alg_req_ind_move_indirectly_movable_subsumption { // libcis: isolate file-scope helpers
template<std::indirectly_readable I, class O>
constexpr bool indirectly_movable_subsumption() {
  return false;
}

template<class I, class O>
  requires std::indirectly_movable<I, O>
constexpr bool indirectly_movable_subsumption() {
  return true;
}

static_assert(indirectly_movable_subsumption<int*, int*>());
} using namespace libcis_ns_iterators_iterator_requirements_alg_req_ind_move_indirectly_movable_subsumption; // libcis

