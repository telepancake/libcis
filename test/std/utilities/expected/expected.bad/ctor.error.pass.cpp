// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_bad_ctor_error).
// main -> test_utilities_expected_expected_bad_ctor_error; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// explicit bad_expected_access(E e);

// Effects: Initializes unex with std::move(e).

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>

#include "test_macros.h"
#include "MoveOnly.h"

// test explicit
namespace libcis_ns_utilities_expected_expected_bad_ctor_error { // libcis: isolate file-scope helpers
static_assert(std::convertible_to<int, int>);
static_assert(!std::convertible_to<int, std::bad_expected_access<int>>);
} using namespace libcis_ns_utilities_expected_expected_bad_ctor_error; // libcis


void test_utilities_expected_expected_bad_ctor_error() {
  std::bad_expected_access<MoveOnly> b(MoveOnly{3});
  assert(b.error().get() == 3);

  return;
}
