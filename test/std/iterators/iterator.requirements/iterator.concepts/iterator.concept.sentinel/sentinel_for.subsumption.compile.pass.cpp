// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_iterator_concepts_iterator_concept_sentinel_sentinel_for_subsumption_774456d4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class S, class I>
// concept sentinel_for;

#include <iterator>

#include <concepts>
#include <vector>

// clang-format off
namespace libcis_ns_iterators_iterator_requirements_iterator_concepts_iterator_concept_sentinel_sentinel_for_subsumption_774456d4 { // libcis
template<std::input_or_output_iterator, std::semiregular>
constexpr bool check_sentinel_subsumption() {
  return false;
}

template<class I, std::sentinel_for<I> >
constexpr bool check_subsumption() {
  return true;
}
// clang-format on

static_assert(check_subsumption<std::vector<int>::iterator, std::vector<int>::iterator>());
} // libcis_ns_iterators_iterator_requirements_iterator_concepts_iterator_concept_sentinel_sentinel_for_subsumption_774456d4 (libcis)

