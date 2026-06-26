// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_iterator_concept_conformance_4f31c5d2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// shared_ptr

#include <memory>

#include <iterator>

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_iterator_concept_conformance_4f31c5d2 { // libcis
static_assert(std::indirectly_readable<std::shared_ptr<int> >);
static_assert(std::indirectly_writable<std::shared_ptr<int>, int>);
static_assert(!std::weakly_incrementable<std::shared_ptr<int> >);
static_assert(std::indirectly_movable<std::shared_ptr<int>, std::shared_ptr<int>>);
static_assert(std::indirectly_movable_storable<std::shared_ptr<int>, std::shared_ptr<int>>);
static_assert(std::indirectly_copyable<std::shared_ptr<int>, std::shared_ptr<int>>);
static_assert(std::indirectly_copyable_storable<std::shared_ptr<int>, std::shared_ptr<int>>);
static_assert(std::indirectly_swappable<std::shared_ptr<int>, std::shared_ptr<int> >);

static_assert(!std::indirectly_readable<std::shared_ptr<void> >);
static_assert(!std::indirectly_writable<std::shared_ptr<void>, void>);
static_assert(!std::weakly_incrementable<std::shared_ptr<void> >);
static_assert(!std::indirectly_movable<std::shared_ptr<void>, std::shared_ptr<void>>);
static_assert(!std::indirectly_movable_storable<std::shared_ptr<void>, std::shared_ptr<void>>);
static_assert(!std::indirectly_copyable<std::shared_ptr<void>, std::shared_ptr<void>>);
static_assert(!std::indirectly_copyable_storable<std::shared_ptr<void>, std::shared_ptr<void>>);
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_iterator_concept_conformance_4f31c5d2 (libcis)

