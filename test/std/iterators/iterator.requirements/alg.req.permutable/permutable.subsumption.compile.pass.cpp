// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_alg_req_permutable_permutable_subsumption_f27ab29a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class I>
//   concept permutable = see below; // Since C++20

#include <iterator>

namespace libcis_ns_iterators_iterator_requirements_alg_req_permutable_permutable_subsumption_f27ab29a { // libcis
template<class I> void test_subsumption() requires std::forward_iterator<I>;
template<class I> void test_subsumption() requires std::indirectly_movable_storable<I, I>;
template<class I> void test_subsumption() requires std::indirectly_swappable<I, I>;
template<class I> constexpr bool test_subsumption() requires std::permutable<I> { return true; }
static_assert(test_subsumption<int*>());
} // libcis_ns_iterators_iterator_requirements_alg_req_permutable_permutable_subsumption_f27ab29a (libcis)

