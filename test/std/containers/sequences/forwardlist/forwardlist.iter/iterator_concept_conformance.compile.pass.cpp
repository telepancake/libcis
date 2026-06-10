// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_iter_iterator_concept_conformance).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// iterator, const_iterator

#include <forward_list>

#include <iterator>

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_iter_iterator_concept_conformance { // libcis: isolate file-scope helpers
using iterator       = std::forward_list<int>::iterator;
using const_iterator = std::forward_list<int>::const_iterator;
using value_type     = int;

static_assert(std::forward_iterator<iterator>);
static_assert(!std::bidirectional_iterator<iterator>);
static_assert(std::indirectly_writable<iterator, value_type>);
static_assert(std::sentinel_for<iterator, iterator>);
static_assert(std::sentinel_for<iterator, const_iterator>);
static_assert(!std::sized_sentinel_for<iterator, iterator>);
static_assert(!std::sized_sentinel_for<iterator, const_iterator>);
static_assert(std::indirectly_movable<iterator, iterator>);
static_assert(std::indirectly_movable_storable<iterator, iterator>);
static_assert(!std::indirectly_movable<iterator, const_iterator>);
static_assert(!std::indirectly_movable_storable<iterator, const_iterator>);
static_assert(std::indirectly_copyable<iterator, iterator>);
static_assert(std::indirectly_copyable_storable<iterator, iterator>);
static_assert(!std::indirectly_copyable<iterator, const_iterator>);
static_assert(!std::indirectly_copyable_storable<iterator, const_iterator>);
static_assert(std::indirectly_swappable<iterator, iterator>);

static_assert(std::forward_iterator<const_iterator>);
static_assert(!std::bidirectional_iterator<const_iterator>);
static_assert(!std::indirectly_writable<const_iterator, value_type>);
static_assert(std::sentinel_for<const_iterator, iterator>);
static_assert(std::sentinel_for<const_iterator, const_iterator>);
static_assert(!std::sized_sentinel_for<const_iterator, iterator>);
static_assert(!std::sized_sentinel_for<const_iterator, const_iterator>);
static_assert(std::indirectly_movable<const_iterator, iterator>);
static_assert(std::indirectly_movable_storable<const_iterator, iterator>);
static_assert(!std::indirectly_movable<const_iterator, const_iterator>);
static_assert(!std::indirectly_movable_storable<const_iterator, const_iterator>);
static_assert(std::indirectly_copyable<const_iterator, iterator>);
static_assert(std::indirectly_copyable_storable<const_iterator, iterator>);
static_assert(!std::indirectly_copyable<const_iterator, const_iterator>);
static_assert(!std::indirectly_copyable_storable<const_iterator, const_iterator>);
static_assert(!std::indirectly_swappable<const_iterator, const_iterator>);
} using namespace libcis_ns_containers_sequences_forwardlist_forwardlist_iter_iterator_concept_conformance; // libcis

