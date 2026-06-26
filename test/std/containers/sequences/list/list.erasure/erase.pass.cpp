// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_erasure_erase_c8b0c408).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <list>

// template <class T, class Allocator, class U>
//   typename list<T, Allocator>::size_type
//   erase(list<T, Allocator>& c, const U& value);

#include <list>
#include <optional>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_list_list_erasure_erase_c8b0c408 { // libcis
template <class S, class U>
void test0(S s, U val, S expected, std::size_t expected_erased_count) {
  ASSERT_SAME_TYPE(typename S::size_type, decltype(std::erase(s, val)));
  assert(expected_erased_count == std::erase(s, val));
  assert(s == expected);
}

template <class S>
void test()
{
  test0(S(), 1, S(), 0);

  test0(S({1}), 1, S(), 1);
  test0(S({1}), 2, S({1}), 0);

  test0(S({1, 2}), 1, S({2}), 1);
  test0(S({1, 2}), 2, S({1}), 1);
  test0(S({1, 2}), 3, S({1, 2}), 0);
  test0(S({1, 1}), 1, S(), 2);
  test0(S({1, 1}), 3, S({1, 1}), 0);

  test0(S({1, 2, 3}), 1, S({2, 3}), 1);
  test0(S({1, 2, 3}), 2, S({1, 3}), 1);
  test0(S({1, 2, 3}), 3, S({1, 2}), 1);
  test0(S({1, 2, 3}), 4, S({1, 2, 3}), 0);

  test0(S({1, 1, 1}), 1, S(), 3);
  test0(S({1, 1, 1}), 2, S({1, 1, 1}), 0);
  test0(S({1, 1, 2}), 1, S({2}), 2);
  test0(S({1, 1, 2}), 2, S({1, 1}), 1);
  test0(S({1, 1, 2}), 3, S({1, 1, 2}), 0);
  test0(S({1, 2, 2}), 1, S({2, 2}), 1);
  test0(S({1, 2, 2}), 2, S({1}), 2);
  test0(S({1, 2, 2}), 3, S({1, 2, 2}), 0);

  //  Test cross-type erasure
  using opt = std::optional<typename S::value_type>;
  test0(S({1, 2, 1}), opt(), S({1, 2, 1}), 0);
  test0(S({1, 2, 1}), opt(1), S({2}), 2);
  test0(S({1, 2, 1}), opt(2), S({1, 1}), 1);
  test0(S({1, 2, 1}), opt(3), S({1, 2, 1}), 0);
}

int main(int, char**)
{
    test<std::list<int>>();
    test<std::list<int, min_allocator<int>>> ();
    test<std::list<int, test_allocator<int>>> ();

    test<std::list<long>>();
    test<std::list<double>>();

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_list_erasure_erase_c8b0c408 (libcis)

