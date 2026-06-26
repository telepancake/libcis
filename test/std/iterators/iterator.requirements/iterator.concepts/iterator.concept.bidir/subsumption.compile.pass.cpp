// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_iterator_concepts_iterator_concept_bidir_subsumption_af3e6092).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
// concept bidirectional_iterator;

#include <iterator>

#include <concepts>

namespace libcis_ns_iterators_iterator_requirements_iterator_concepts_iterator_concept_bidir_subsumption_af3e6092 { // libcis
template<std::forward_iterator I>
constexpr bool check_subsumption() {
  return false;
}

template<std::bidirectional_iterator>
constexpr bool check_subsumption() {
  return true;
}

static_assert(check_subsumption<int*>());
} // libcis_ns_iterators_iterator_requirements_iterator_concepts_iterator_concept_bidir_subsumption_af3e6092 (libcis)

