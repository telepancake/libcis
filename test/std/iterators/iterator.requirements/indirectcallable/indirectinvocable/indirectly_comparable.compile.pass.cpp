// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_indirectcallable_indirectinvocable_indirectly_comparable_b3c8c1e6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class I1, class I2, class R, class P1, class P2>
// concept indirectly_comparable;

#include <functional>
#include <iterator>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_iterators_iterator_requirements_indirectcallable_indirectinvocable_indirectly_comparable_b3c8c1e6 { // libcis
struct Deref {
    int operator()(int*) const;
};

static_assert(!std::indirectly_comparable<int, int, std::less<int>>);  // not dereferenceable
static_assert(!std::indirectly_comparable<int*, int*, int>);  // not a predicate
static_assert( std::indirectly_comparable<int*, int*, std::less<int>>);
static_assert(!std::indirectly_comparable<int**, int*, std::less<int>>);
static_assert( std::indirectly_comparable<int**, int*, std::less<int>, Deref>);
static_assert(!std::indirectly_comparable<int**, int*, std::less<int>, Deref, Deref>);
static_assert(!std::indirectly_comparable<int**, int*, std::less<int>, std::identity, Deref>);
static_assert( std::indirectly_comparable<int*, int**, std::less<int>, std::identity, Deref>);

template<class F>
  requires std::indirectly_comparable<int*, char*, F>
           && true // This true is an additional atomic constraint as a tie breaker
constexpr bool subsumes(F) { return true; }

template<class F>
  requires std::indirect_binary_predicate<F, std::projected<int*, std::identity>, std::projected<char*, std::identity>>
void subsumes(F);

template<class F>
  requires std::indirect_binary_predicate<F, std::projected<int*, std::identity>, std::projected<char*, std::identity>>
           && true // This true is an additional atomic constraint as a tie breaker
constexpr bool is_subsumed(F) { return true; }

template<class F>
  requires std::indirectly_comparable<int*, char*, F>
void is_subsumed(F);

static_assert(subsumes(std::less<int>()));
static_assert(is_subsumed(std::less<int>()));

// Test ADL-proofing (P2538R1)
#if TEST_STD_VER >= 26 || defined(_LIBCPP_VERSION)
struct Incomplete;
template<class T> struct Holder { T t; };
static_assert(std::indirectly_comparable<Holder<Incomplete>**, Holder<Incomplete>**, std::less<Holder<Incomplete>*>>);
static_assert(!std::indirectly_comparable<Holder<Incomplete>**, Holder<Incomplete>**, Holder<Incomplete>*>);
#endif

} // libcis_ns_iterators_iterator_requirements_indirectcallable_indirectinvocable_indirectly_comparable_b3c8c1e6 (libcis)
