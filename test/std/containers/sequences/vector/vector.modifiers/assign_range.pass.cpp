// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_modifiers_assign_range).
// main -> test_containers_sequences_vector_vector_modifiers_assign_range; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20
// ADDITIONAL_COMPILE_FLAGS(has-fconstexpr-steps): -fconstexpr-steps=2000000

// template<container-compatible-range<T> R>
//   constexpr void assign_range(R&& rg); // C++23

#include <sstream>
#include <vector>

#include "../../insert_range_sequence_containers.h"
#include "asan_testing.h"
#include "test_macros.h"

// Tested cases:
// - different kinds of assignments (assigning an {empty/one-element/mid-sized/long range} to an
//   {empty/one-element/full} container);
// - assigning move-only elements;
// - an exception is thrown when copying the elements or when allocating new elements.
namespace libcis_ns_containers_sequences_vector_vector_modifiers_assign_range { // libcis: isolate file-scope helpers
constexpr bool test() {
  static_assert(test_constraints_assign_range<std::vector, int, double>());

  for_all_iterators_and_allocators<int, const int*>([]<class Iter, class Sent, class Alloc>() {
    test_sequence_assign_range<std::vector<int, Alloc>, Iter, Sent>([]([[maybe_unused]] auto&& c) {
      LIBCPP_ASSERT(c.__invariants());
      LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
    });
  });
  test_sequence_assign_range_move_only<std::vector>();

  {   // Vector may or may not need to reallocate because of the assignment -- make sure to test both cases.
    { // Ensure reallocation happens.
      int in[]           = {-1, -2, -3, -4, -5, -6, -7, -8, -9, -10};
      std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
      v.shrink_to_fit();
      assert(v.capacity() < v.size() + std::ranges::size(in));

      v.assign_range(in);
      assert(std::ranges::equal(v, in));
    }

    { // Ensure no reallocation happens -- the input range is shorter than the vector.
      int in[]           = {-1, -2, -3, -4, -5};
      std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

      v.assign_range(in);
      assert(std::ranges::equal(v, in));
    }

    { // Ensure no reallocation happens -- the input range is longer than the vector but within capacity.
      int in[]           = {-1, -2, -3, -4, -5, -6, -7, -8};
      std::vector<int> v = {1, 2, 3, 4, 5};
      v.reserve(std::ranges::size(in));
      assert(v.capacity() >= std::ranges::size(in));

      v.assign_range(in);
      assert(std::ranges::equal(v, in));
    }

    { // Ensure input-only sized ranges are accepted.
      using input_iter = cpp20_input_iterator<const int*>;
      const int in[]{1, 2, 3, 4};
      std::vector<int> v;
      v.assign_range(std::views::counted(input_iter{std::ranges::begin(in)}, std::ranges::ssize(in)));
      assert(std::ranges::equal(v, std::vector<int>{1, 2, 3, 4}));
    }
  }

  return true;
}

#ifndef TEST_HAS_NO_LOCALIZATION
void test_counted_istream_view() {
  std::istringstream is{"1 2 3 4"};
  auto vals = std::views::istream<int>(is);
  std::vector<int> v;
  v.assign_range(std::views::counted(vals.begin(), 3));
  assert(v == (std::vector{1, 2, 3}));
}
} using namespace libcis_ns_containers_sequences_vector_vector_modifiers_assign_range; // libcis

#endif

void test_containers_sequences_vector_vector_modifiers_assign_range() {
  test();
  static_assert(test());

  test_assign_range_exception_safety_throwing_copy<std::vector>();
  test_assign_range_exception_safety_throwing_allocator<std::vector, int>();

#ifndef TEST_HAS_NO_LOCALIZATION
  test_counted_istream_view();
#endif

  return;
}
