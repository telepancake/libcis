// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istreambuf_iterator_iterator_concept_conformance).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// istreambuf_iterator

#include <iterator>

#include <istream>
#include <string>

namespace libcis_ns_iterators_stream_iterators_istreambuf_iterator_iterator_concept_conformance { // libcis: isolate file-scope helpers
using iterator = std::istreambuf_iterator<char>;

static_assert(std::input_iterator<iterator>);
static_assert(!std::forward_iterator<iterator>);
static_assert(!std::indirectly_writable<iterator, char>);
static_assert(!std::incrementable<iterator>);
static_assert(std::sentinel_for<iterator, iterator>);
static_assert(!std::sized_sentinel_for<iterator, iterator>);
static_assert(std::indirectly_movable<iterator, char*>);
static_assert(std::indirectly_movable_storable<iterator, char*>);
static_assert(std::indirectly_copyable<iterator, char*>);
static_assert(std::indirectly_copyable_storable<iterator, char*>);
static_assert(!std::indirectly_swappable<iterator, iterator>);
} using namespace libcis_ns_iterators_stream_iterators_istreambuf_iterator_iterator_concept_conformance; // libcis

