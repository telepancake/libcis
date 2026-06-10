// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_default_allocator_PR50299).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// Make sure we can use std::allocator<void> in all Standard modes. While the
// explicit specialization for std::allocator<void> was deprecated, using that
// specialization was neither deprecated nor removed (in C++20 it should simply
// start using the primary template).
//
// See https://llvm.org/PR50299.

#include <memory>

namespace libcis_ns_utilities_memory_default_allocator_PR50299 { // libcis: isolate file-scope helpers
std::allocator<void> a;
} using namespace libcis_ns_utilities_memory_default_allocator_PR50299; // libcis

