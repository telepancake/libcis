// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_alg_req_ind_swap_indirectly_swappable_subsumption_a2671105).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class I1, class I2>
// concept indirectly_swappable;

#include <iterator>

#include <concepts>

namespace libcis_ns_iterators_iterator_requirements_alg_req_ind_swap_indirectly_swappable_subsumption_a2671105 { // libcis
template<class I1, class I2>
  requires std::indirectly_readable<I1> && std::indirectly_readable<I2>
constexpr bool indirectly_swappable_subsumption() {
  return false;
}

template<class I1, class I2>
  requires std::indirectly_swappable<I1, I2>
constexpr bool indirectly_swappable_subsumption() {
  return true;
}

static_assert(indirectly_swappable_subsumption<int*, int*>());
} // libcis_ns_iterators_iterator_requirements_alg_req_ind_swap_indirectly_swappable_subsumption_a2671105 (libcis)

