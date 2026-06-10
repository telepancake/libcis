// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_move_sentinel_ctad).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <iterator>
#include <type_traits>

#include <test_iterators.h>

namespace libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_ctad { // libcis: isolate file-scope helpers
static_assert(std::is_same_v<decltype(std::move_sentinel(std::default_sentinel_t{})),
                             std::move_sentinel<std::default_sentinel_t>>);

static_assert(
    std::is_same_v<decltype(std::move_sentinel(sentinel_wrapper<int*>{})), std::move_sentinel<sentinel_wrapper<int*>>>);
} using namespace libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_ctad; // libcis

