// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_iterator_concepts_iterator_concept_iterator_subsumption_d7b3f957).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class In>
// concept input_or_output_iterator;

#include <iterator>

// clang-format off
namespace libcis_ns_iterators_iterator_requirements_iterator_concepts_iterator_concept_iterator_subsumption_d7b3f957 { // libcis
template<std::weakly_incrementable>
constexpr bool check_subsumption() {
  return false;
}

template<std::input_or_output_iterator>
constexpr bool check_subsumption() {
  return true;
}
// clang-format on

static_assert(check_subsumption<int*>());
} // libcis_ns_iterators_iterator_requirements_iterator_concepts_iterator_concept_iterator_subsumption_d7b3f957 (libcis)

