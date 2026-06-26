// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_allocator_adaptor_allocator_adaptor_types_propagate_on_container_move_assignment_8aba9c96).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// typedef see below propagate_on_container_move_assignment;

#include <scoped_allocator>
#include <type_traits>

#include "test_macros.h"
#include "allocators.h"

namespace libcis_ns_utilities_allocator_adaptor_allocator_adaptor_types_propagate_on_container_move_assignment_8aba9c96 { // libcis
int main(int, char**) {
  static_assert((std::is_same< std::scoped_allocator_adaptor<A1<int>>::propagate_on_container_move_assignment,
                               std::false_type>::value),
                "");

  static_assert((std::is_same< std::scoped_allocator_adaptor<A1<int>, A2<int>>::propagate_on_container_move_assignment,
                               std::true_type>::value),
                "");

  static_assert(
      (std::is_same< std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>>::propagate_on_container_move_assignment,
                     std::true_type>::value),
      "");

  return 0;

    return 0;
}
} // libcis_ns_utilities_allocator_adaptor_allocator_adaptor_types_propagate_on_container_move_assignment_8aba9c96 (libcis)

