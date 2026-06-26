// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_iter_re_tokiter_iterator_concept_conformance_a18f197e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// regex_token_iterator

#include <regex>

#include <iterator>

namespace libcis_ns_re_re_iter_re_tokiter_iterator_concept_conformance_a18f197e { // libcis
static_assert(std::input_iterator<std::cregex_token_iterator>);
static_assert(!std::forward_iterator<std::cregex_token_iterator>);
static_assert(!std::indirectly_writable<std::cregex_token_iterator, char>);
static_assert(std::sentinel_for<std::cregex_token_iterator, std::cregex_token_iterator>);
static_assert(!std::sized_sentinel_for<std::cregex_token_iterator, std::cregex_token_iterator>);
static_assert(!std::indirectly_movable<std::cregex_token_iterator, std::cregex_token_iterator>);
static_assert(!std::indirectly_movable_storable<std::cregex_token_iterator, std::cregex_token_iterator>);
static_assert(!std::indirectly_copyable<std::cregex_token_iterator, std::cregex_token_iterator>);
static_assert(!std::indirectly_copyable_storable<std::cregex_token_iterator, std::cregex_token_iterator>);
static_assert(!std::indirectly_swappable<std::cregex_token_iterator, std::cregex_token_iterator>);
} // libcis_ns_re_re_iter_re_tokiter_iterator_concept_conformance_a18f197e (libcis)

