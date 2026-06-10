// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_explicit_conversion).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_explicit_conversion; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Make sure that std::allocate_shared works with an allocator type that is
// only explicitly convertible from another specialization of itself.

#include <cassert>
#include <cstddef>
#include <memory>

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_explicit_conversion { // libcis: isolate file-scope helpers
template <class T>
struct ExplicitAllocator {
    ExplicitAllocator() = default;
    template <class U>
    explicit ExplicitAllocator(ExplicitAllocator<U>) { }

    using value_type = T;
    T* allocate(std::size_t n) { return std::allocator<T>().allocate(n); }
    void deallocate(T* ptr, std::size_t n) { return std::allocator<T>().deallocate(ptr, n); }
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_explicit_conversion; // libcis


void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_allocate_shared_explicit_conversion() {
    std::shared_ptr<int> ptr = std::allocate_shared<int>(ExplicitAllocator<int>(), 0);
    (void)ptr;

    return;
}
