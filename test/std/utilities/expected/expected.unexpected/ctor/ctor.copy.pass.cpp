// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_unexpected_ctor_ctor_copy).
// main -> test_utilities_expected_expected_unexpected_ctor_ctor_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr unexpected(const unexpected&) = default;

#include <cassert>
#include <expected>

namespace libcis_ns_utilities_expected_expected_unexpected_ctor_ctor_copy { // libcis: isolate file-scope helpers
struct Error {
  int i;
  constexpr Error(int ii) : i(ii) {}
};

constexpr bool test() {
  const std::unexpected<Error> unex(5);
  auto unex2 = unex;
  assert(unex2.error().i == 5);
  return true;
}
} using namespace libcis_ns_utilities_expected_expected_unexpected_ctor_ctor_copy; // libcis


void test_utilities_expected_expected_unexpected_ctor_ctor_copy() {
  test();
  static_assert(test());
  return;
}
