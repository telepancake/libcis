// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_ignore_ignore_include).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <utility>

// inline constexpr ignore-type ignore;

// std::ignore should be provided by the headers <tuple> and <utility>.
// This test validates its presence in <utility>.

#include <utility>

[[maybe_unused]] namespace libcis_ns_utilities_utility_ignore_ignore_include { // libcis: isolate file-scope helpers
auto& ignore_v = std::ignore;
} using namespace libcis_ns_utilities_utility_ignore_ignore_include; // libcis

