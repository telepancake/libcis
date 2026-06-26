// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap_7f73f6b1).
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

namespace libcis_ns_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap_7f73f6b1 { // libcis
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

int main(int, char**)
{
    static_assert((std::is_same<std::allocator_traits<A<char> >::propagate_on_container_swap, std::true_type>::value), "");
    static_assert((std::is_same<std::allocator_traits<B<char> >::propagate_on_container_swap, std::false_type>::value), "");
 #if TEST_STD_VER >= 11
    static_assert((std::is_same<std::allocator_traits<C<char> >::propagate_on_container_swap, std::false_type>::value), "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap_7f73f6b1 (libcis)

