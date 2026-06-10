// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_cons_nullptr).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <string>

// basic_string(nullptr_t) = delete; // C++23
// basic_string& operator=(nullptr_t) = delete; // C++23

#include <string>
#include <type_traits>

namespace libcis_ns_strings_basic_string_string_cons_nullptr { // libcis: isolate file-scope helpers
static_assert(!std::is_convertible_v<decltype(nullptr), std::string>);
static_assert(!std::is_constructible_v<std::string, decltype(nullptr)>);
static_assert(!std::is_assignable_v<std::string, decltype(nullptr)>);
} using namespace libcis_ns_strings_basic_string_string_cons_nullptr; // libcis

