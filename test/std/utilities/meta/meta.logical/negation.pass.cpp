// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_logical_negation).
// main -> test_utilities_meta_meta_logical_negation; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// type_traits

// template<class B> struct negation;                        // C++17
// template<class B>
//   constexpr bool negation_v = negation<B>::value;         // C++17

#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_logical_negation { // libcis: isolate file-scope helpers
struct True  { static constexpr bool value = true; };
struct False { static constexpr bool value = false; };
} using namespace libcis_ns_utilities_meta_meta_logical_negation; // libcis


void test_utilities_meta_meta_logical_negation()
{
    static_assert (!std::negation<std::true_type >::value, "" );
    static_assert ( std::negation<std::false_type>::value, "" );

    static_assert (!std::negation_v<std::true_type >, "" );
    static_assert ( std::negation_v<std::false_type>, "" );

    static_assert (!std::negation<True >::value, "" );
    static_assert ( std::negation<False>::value, "" );

    static_assert (!std::negation_v<True >, "" );
    static_assert ( std::negation_v<False>, "" );

    static_assert ( std::negation<std::negation<std::true_type >>::value, "" );
    static_assert (!std::negation<std::negation<std::false_type>>::value, "" );

  return;
}
