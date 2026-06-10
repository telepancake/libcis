// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_allocator_traits_value_type).
// main -> test_utilities_memory_allocator_traits_value_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class Alloc>
// struct allocator_traits
// {
//     typedef typename Alloc::value_type value_type;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_allocator_traits_value_type { // libcis: isolate file-scope helpers
template <class T>
struct A
{
    typedef T value_type;
};
} using namespace libcis_ns_utilities_memory_allocator_traits_value_type; // libcis


void test_utilities_memory_allocator_traits_value_type()
{
    static_assert((std::is_same<std::allocator_traits<A<char> >::value_type, char>::value), "");

  return;
}
