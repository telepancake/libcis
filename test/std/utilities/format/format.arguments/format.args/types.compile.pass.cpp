// AST-transferred from libc++ by tools/transfer.py (slug=utilities_format_format_arguments_format_args_types).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <format>

// Namespace std typedefs:
// using format_args = basic_format_args<format_context>;
// using wformat_args = basic_format_args<wformat_context>;

#include <format>
#include <vector>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_format_format_arguments_format_args_types { // libcis: isolate file-scope helpers
static_assert(std::is_same_v<std::format_args,
                             std::basic_format_args<std::format_context>>);
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
static_assert(std::is_same_v<std::wformat_args,
                             std::basic_format_args<std::wformat_context>>);
} using namespace libcis_ns_utilities_format_format_arguments_format_args_types; // libcis

#endif
