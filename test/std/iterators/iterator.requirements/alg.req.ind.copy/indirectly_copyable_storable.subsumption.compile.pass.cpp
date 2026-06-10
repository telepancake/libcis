// AST-transferred from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_alg_req_ind_copy_indirectly_copyable_storable_subsumption).
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
// concept indirectly_copyable_storable;

#include <iterator>

namespace libcis_ns_iterators_iterator_requirements_alg_req_ind_copy_indirectly_copyable_storable_subsumption { // libcis: isolate file-scope helpers
template<class I, class O>
  requires std::indirectly_copyable<I, O>
constexpr bool indirectly_copyable_storable_subsumption() {
  return false;
}

template<class I, class O>
  requires std::indirectly_copyable_storable<I, O>
constexpr bool indirectly_copyable_storable_subsumption() {
  return true;
}

static_assert(indirectly_copyable_storable_subsumption<int*, int*>());
} using namespace libcis_ns_iterators_iterator_requirements_alg_req_ind_copy_indirectly_copyable_storable_subsumption; // libcis

