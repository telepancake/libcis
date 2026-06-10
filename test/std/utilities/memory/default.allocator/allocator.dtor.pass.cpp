// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_default_allocator_allocator_dtor).
// main -> test_utilities_memory_default_allocator_allocator_dtor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template <class T>
// constexpr allocator<T>::~allocator();

#include <memory>

namespace libcis_ns_utilities_memory_default_allocator_allocator_dtor { // libcis: isolate file-scope helpers
template <typename T>
constexpr bool test() {
    std::allocator<T> alloc;
    (void)alloc;

    // destructor called here
    return true;
}
} using namespace libcis_ns_utilities_memory_default_allocator_allocator_dtor; // libcis


void test_utilities_memory_default_allocator_allocator_dtor()
{
    test<int>();
    test<void>();

    static_assert(test<int>());
    static_assert(test<void>());

    return;
}
