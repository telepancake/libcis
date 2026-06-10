// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_array_aggregate).
// main -> test_containers_sequences_array_aggregate; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Make sure std::array is an aggregate type.
// We can only check this in C++17 and above, because we don't have the
// trait before that.
// UNSUPPORTED: c++03, c++11, c++14

#include <array>
#include <type_traits>

namespace libcis_ns_containers_sequences_array_aggregate { // libcis: isolate file-scope helpers
template <typename T>
void check_aggregate() {
  static_assert(std::is_aggregate<std::array<T, 0> >::value, "");
  static_assert(std::is_aggregate<std::array<T, 1> >::value, "");
  static_assert(std::is_aggregate<std::array<T, 2> >::value, "");
  static_assert(std::is_aggregate<std::array<T, 3> >::value, "");
  static_assert(std::is_aggregate<std::array<T, 4> >::value, "");
}

struct Empty {};
struct Trivial {
  int i;
  int j;
};
struct NonTrivial {
  int i;
  int j;
  NonTrivial(NonTrivial const&) {}
};
} using namespace libcis_ns_containers_sequences_array_aggregate; // libcis


void test_containers_sequences_array_aggregate() {
  check_aggregate<char>();
  check_aggregate<int>();
  check_aggregate<long>();
  check_aggregate<float>();
  check_aggregate<double>();
  check_aggregate<long double>();
  check_aggregate<Empty>();
  check_aggregate<Trivial>();
  check_aggregate<NonTrivial>();

  return;
}
