// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_bad_base).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// Make sure std::bad_expected_access<E> inherits from std::bad_expected_access<void>.

#include <expected>
#include <type_traits>

namespace libcis_ns_utilities_expected_expected_bad_base { // libcis: isolate file-scope helpers
struct Foo {};

static_assert(std::is_base_of_v<std::bad_expected_access<void>, std::bad_expected_access<int>>);
static_assert(std::is_base_of_v<std::bad_expected_access<void>, std::bad_expected_access<Foo>>);
} using namespace libcis_ns_utilities_expected_expected_bad_base; // libcis

