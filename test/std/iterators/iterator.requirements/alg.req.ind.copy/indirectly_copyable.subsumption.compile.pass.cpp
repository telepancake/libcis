// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_alg_req_ind_copy_indirectly_copyable_subsumption_0837e7eb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class In, class Out>
// concept indirectly_copyable;

#include <iterator>

namespace libcis_ns_iterators_iterator_requirements_alg_req_ind_copy_indirectly_copyable_subsumption_0837e7eb { // libcis
template<std::indirectly_readable I, class O>
constexpr bool indirectly_copyable_subsumption() {
  return false;
}

template<class I, class O>
  requires std::indirectly_copyable<I, O>
constexpr bool indirectly_copyable_subsumption() {
  return true;
}

static_assert(indirectly_copyable_subsumption<int*, int*>());
} // libcis_ns_iterators_iterator_requirements_alg_req_ind_copy_indirectly_copyable_subsumption_0837e7eb (libcis)

