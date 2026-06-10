// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap).
// main -> test_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap; file-scope helpers isolated in anon namespace.
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
//     typedef Alloc::propagate_on_container_swap
//           | false_type                   propagate_on_container_swap;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap { // libcis: isolate file-scope helpers
template <class T>
struct A
{
    typedef T value_type;
    typedef std::true_type propagate_on_container_swap;
};

template <class T>
struct B
{
    typedef T value_type;
};

template <class T>
struct C
{
    typedef T value_type;
private:
    typedef std::true_type propagate_on_container_swap;
};
} using namespace libcis_ns_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap; // libcis


void test_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap()
{
    static_assert((std::is_same<std::allocator_traits<A<char> >::propagate_on_container_swap, std::true_type>::value), "");
    static_assert((std::is_same<std::allocator_traits<B<char> >::propagate_on_container_swap, std::false_type>::value), "");
 #if TEST_STD_VER >= 11
    static_assert((std::is_same<std::allocator_traits<C<char> >::propagate_on_container_swap, std::false_type>::value), "");
#endif

  return;
}
