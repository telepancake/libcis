// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_unexpected_assign_assign_move).
// main -> test_utilities_expected_expected_unexpected_assign_assign_move; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr unexpected& operator=(unexpected&&) = default;

#include <cassert>
#include <expected>
#include <utility>

namespace libcis_ns_utilities_expected_expected_unexpected_assign_assign_move { // libcis: isolate file-scope helpers
struct Error {
  int i;
  constexpr Error(int ii) : i(ii) {}
  constexpr Error& operator=(Error&& other) {
    i       = other.i;
    other.i = 0;
    return *this;
  }
};

constexpr bool test() {
  std::unexpected<Error> unex1(4);
  std::unexpected<Error> unex2(5);
  unex1 = std::move(unex2);
  assert(unex1.error().i == 5);
  assert(unex2.error().i == 0);
  return true;
}
} using namespace libcis_ns_utilities_expected_expected_unexpected_assign_assign_move; // libcis


void test_utilities_expected_expected_unexpected_assign_assign_move() {
  test();
  static_assert(test());
  return;
}
