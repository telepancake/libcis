// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_unique_ptr_iterator_concept_conformance).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// unique_ptr

#include <memory>

#include <iterator>

namespace libcis_ns_utilities_memory_unique_ptr_iterator_concept_conformance { // libcis: isolate file-scope helpers
static_assert(std::indirectly_readable<std::unique_ptr<int> >);
static_assert(std::indirectly_writable<std::unique_ptr<int>, int>);
static_assert(!std::weakly_incrementable<std::unique_ptr<int> >);
static_assert(std::indirectly_movable<std::unique_ptr<int>, std::unique_ptr<int>>);
static_assert(std::indirectly_movable_storable<std::unique_ptr<int>, std::unique_ptr<int>>);
static_assert(std::indirectly_copyable<std::unique_ptr<int>, std::unique_ptr<int>>);
static_assert(std::indirectly_copyable_storable<std::unique_ptr<int>, std::unique_ptr<int>>);
static_assert(std::indirectly_swappable<std::unique_ptr<int>, std::unique_ptr<int> >);
} using namespace libcis_ns_utilities_memory_unique_ptr_iterator_concept_conformance; // libcis

