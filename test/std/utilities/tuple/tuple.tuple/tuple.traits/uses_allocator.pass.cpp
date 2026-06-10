// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_traits_uses_allocator).
// main -> test_utilities_tuple_tuple_tuple_tuple_traits_uses_allocator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template <class... Types, class Alloc>
//   struct uses_allocator<tuple<Types...>, Alloc> : true_type { };

// UNSUPPORTED: c++03

#include <tuple>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_traits_uses_allocator { // libcis: isolate file-scope helpers
struct A {};
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_traits_uses_allocator; // libcis


void test_utilities_tuple_tuple_tuple_tuple_traits_uses_allocator()
{
    {
        typedef std::tuple<> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }
    {
        typedef std::tuple<int> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }
    {
        typedef std::tuple<char, int> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }
    {
        typedef std::tuple<double&, char, int> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }

  return;
}
