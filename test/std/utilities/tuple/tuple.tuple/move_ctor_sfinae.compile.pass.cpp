// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_move_ctor_sfinae).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Ensure that tuple's move constructor properly SFINAES.
// This is a regression test for https://github.com/llvm/llvm-project/pull/151654#issuecomment-3205410955

// UNSUPPORTED: c++03, c++11, c++14

#include <tuple>
#include <variant>
#include <type_traits>

namespace libcis_ns_utilities_tuple_tuple_tuple_move_ctor_sfinae { // libcis: isolate file-scope helpers
struct S {
  S(const S&)            = delete;
  S& operator=(const S&) = delete;
  S(S&&)                 = default;
  S& operator=(S&&)      = default;
};

using T = std::tuple<const std::variant<S>>;

void func() { (void)std::is_trivially_move_constructible<T>::value; }
} using namespace libcis_ns_utilities_tuple_tuple_tuple_move_ctor_sfinae; // libcis

