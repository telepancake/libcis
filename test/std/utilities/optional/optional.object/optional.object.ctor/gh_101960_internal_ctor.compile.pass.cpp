// AST-transferred from libc++ by tools/transfer.py (slug=utilities_optional_optional_object_optional_object_ctor_gh_101960_internal_ctor).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <optional>

// Regression test for https://llvm.org/PR101960 where a constructor
// of std::optional that should have been private was instead publicly available.

#include <optional>
#include <type_traits>

namespace libcis_ns_utilities_optional_optional_object_optional_object_ctor_gh_101960_internal_ctor { // libcis: isolate file-scope helpers
struct NastyConvertible {
  template <class T>
  operator T() {
    return 0;
  }
};

using F = int(int);

static_assert(!std::is_constructible<std::optional<int>, NastyConvertible, int(int), int>::value);
} using namespace libcis_ns_utilities_optional_optional_object_optional_object_ctor_gh_101960_internal_ctor; // libcis

