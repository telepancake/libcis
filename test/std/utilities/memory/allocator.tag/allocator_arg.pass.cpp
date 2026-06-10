// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_allocator_tag_allocator_arg).
// main -> test_utilities_memory_allocator_tag_allocator_arg; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// struct allocator_arg_t { explicit allocator_arg_t() = default; };
// const allocator_arg_t allocator_arg = allocator_arg_t();

#include <memory>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_allocator_tag_allocator_arg { // libcis: isolate file-scope helpers
void test(std::allocator_arg_t) {}
} using namespace libcis_ns_utilities_memory_allocator_tag_allocator_arg; // libcis


void test_utilities_memory_allocator_tag_allocator_arg()
{
    test(std::allocator_arg);

    return;
}
