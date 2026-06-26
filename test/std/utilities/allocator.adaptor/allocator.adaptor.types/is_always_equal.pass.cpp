// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_allocator_adaptor_allocator_adaptor_types_is_always_equal_d2fc631a).
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

// typedef see below is_always_equal;

#include <scoped_allocator>
#include <type_traits>

#include "test_macros.h"
#include "allocators.h"
#include "min_allocator.h"

namespace libcis_ns_utilities_allocator_adaptor_allocator_adaptor_types_is_always_equal_d2fc631a { // libcis
int main(int, char**) {
  // sanity checks
  static_assert((std::is_same< std::allocator_traits<A1<int>>::is_always_equal, std::false_type>::value), "");

  static_assert((std::is_same< std::allocator_traits<min_allocator<int>>::is_always_equal, std::true_type>::value), "");

  // wrapping one allocator
  static_assert((std::is_same< std::scoped_allocator_adaptor<A1<int>>::is_always_equal,
                               std::allocator_traits<A1<int>>::is_always_equal >::value),
                "");

  // wrapping one allocator
  static_assert((std::is_same< std::scoped_allocator_adaptor<min_allocator<int>>::is_always_equal,
                               std::allocator_traits<min_allocator<int>>::is_always_equal >::value),
                "");

  // wrapping two allocators (check the values instead of the types)
  static_assert((std::scoped_allocator_adaptor<A1<int>, A2<int>>::is_always_equal::value ==
                 (std::allocator_traits<A1<int>>::is_always_equal::value &&
                  std::allocator_traits<A2<int>>::is_always_equal::value)),
                "");

  // wrapping two allocators (check the values instead of the types)
  static_assert((std::scoped_allocator_adaptor<A1<int>, min_allocator<int>>::is_always_equal::value ==
                 (std::allocator_traits<A1<int>>::is_always_equal::value &&
                  std::allocator_traits<min_allocator<int>>::is_always_equal::value)),
                "");

  // wrapping three allocators (check the values instead of the types)
  static_assert((std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>>::is_always_equal::value ==
                 (std::allocator_traits<A1<int>>::is_always_equal::value &&
                  std::allocator_traits<A2<int>>::is_always_equal::value &&
                  std::allocator_traits<A3<int>>::is_always_equal::value)),
                "");

  return 0;

    return 0;
}
} // libcis_ns_utilities_allocator_adaptor_allocator_adaptor_types_is_always_equal_d2fc631a (libcis)

