// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_iterator_requirements_indirectcallable_indirectinvocable_indirect_unary_predicate_83c564e5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class F, class I>
// concept indirect_unary_predicate;

#include <iterator>
#include <type_traits>

#include "indirectly_readable.h"
#include "test_macros.h"

namespace libcis_ns_iterators_iterator_requirements_indirectcallable_indirectinvocable_indirect_unary_predicate_83c564e5 { // libcis
using It = IndirectlyReadable<struct Token>;

template <class I>
struct GoodPredicate {
    bool operator()(std::iter_reference_t<I>) const;
    bool operator()(std::iter_value_t<I>&) const;
    bool operator()(std::iter_common_reference_t<I>) const;
};

// Should work when all constraints are satisfied
static_assert(std::indirect_unary_predicate<GoodPredicate<It>, It>);
static_assert(std::indirect_unary_predicate<bool(*)(int), int*>);
[[maybe_unused]] auto lambda = [](int i) { return i % 2 == 0; };
static_assert(std::indirect_unary_predicate<decltype(lambda), int*>);

// Should fail when the iterator is not indirectly_readable
struct NotIndirectlyReadable { };
static_assert(!std::indirect_unary_predicate<GoodPredicate<NotIndirectlyReadable>, NotIndirectlyReadable>);

// Should fail when the predicate is not copy constructible
struct BadPredicate1 {
    BadPredicate1(BadPredicate1 const&) = delete;
    template <class T> bool operator()(T const&) const;
};
static_assert(!std::indirect_unary_predicate<BadPredicate1, It>);

// Should fail when the predicate can't be called with std::iter_value_t<It>&
struct BadPredicate2 {
    template <class T> bool operator()(T const&) const;
    bool operator()(std::iter_value_t<It>&) const = delete;
};
static_assert(!std::indirect_unary_predicate<BadPredicate2, It>);

// Should fail when the predicate can't be called with std::iter_reference_t<It>
struct BadPredicate3 {
    template <class T> bool operator()(T const&) const;
    bool operator()(std::iter_reference_t<It>) const = delete;
};
static_assert(!std::indirect_unary_predicate<BadPredicate3, It>);

// This case was made valid by P2997R1.
struct GoodPredicate4 {
  template <class T>
  bool operator()(T const&) const;
  bool operator()(std::iter_common_reference_t<It>) const = delete;
};
static_assert(std::indirect_unary_predicate<GoodPredicate4, It>);

// Test ADL-proofing (P2538R1)
#if TEST_STD_VER >= 26 || defined(_LIBCPP_VERSION)
struct Incomplete;
template<class T> struct Holder { T t; };
struct HolderIncompletePred { bool operator()(Holder<Incomplete>*) const; };
static_assert(std::indirect_unary_predicate<HolderIncompletePred, Holder<Incomplete>**>);
static_assert(!std::indirect_unary_predicate<Holder<Incomplete>*, Holder<Incomplete>**>);
#endif

} // libcis_ns_iterators_iterator_requirements_indirectcallable_indirectinvocable_indirect_unary_predicate_83c564e5 (libcis)
