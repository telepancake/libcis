// AST-transferred from libc++ by tools/transfer.py (slug=re_re_iter_re_regiter_iterator_concept_conformance).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// regex_iterator

#include <regex>

#include <iterator>

namespace libcis_ns_re_re_iter_re_regiter_iterator_concept_conformance { // libcis: isolate file-scope helpers
static_assert(std::input_iterator<std::cregex_iterator>);
static_assert(!std::forward_iterator<std::cregex_iterator>);
static_assert(!std::indirectly_writable<std::cregex_iterator, char>);
static_assert(std::sentinel_for<std::cregex_iterator, std::cregex_iterator>);
static_assert(!std::sized_sentinel_for<std::cregex_iterator, std::cregex_iterator>);
static_assert(!std::indirectly_movable<std::cregex_iterator, std::cregex_iterator>);
static_assert(!std::indirectly_movable_storable<std::cregex_iterator, std::cregex_iterator>);
static_assert(!std::indirectly_copyable<std::cregex_iterator, std::cregex_iterator>);
static_assert(!std::indirectly_copyable_storable<std::cregex_iterator, std::cregex_iterator>);
static_assert(!std::indirectly_swappable<std::cregex_iterator, std::cregex_iterator>);
} using namespace libcis_ns_re_re_iter_re_regiter_iterator_concept_conformance; // libcis

