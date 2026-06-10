// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_helper_tuple_size).
// main -> test_utilities_tuple_tuple_tuple_tuple_helper_tuple_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template <class... Types>
//   class tuple_size<tuple<Types...>>
//     : public integral_constant<size_t, sizeof...(Types)> { };

// UNSUPPORTED: c++03

#include <tuple>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_helper_tuple_size { // libcis: isolate file-scope helpers
template <class T, std::size_t N>
void test()
{
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<const T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<volatile T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<const volatile T> >::value), "");
}
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_helper_tuple_size; // libcis


void test_utilities_tuple_tuple_tuple_tuple_helper_tuple_size()
{
    test<std::tuple<>, 0>();
    test<std::tuple<int>, 1>();
    test<std::tuple<char, int>, 2>();
    test<std::tuple<char, char*, int>, 3>();

  return;
}
