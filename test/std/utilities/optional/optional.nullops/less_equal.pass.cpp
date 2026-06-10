// AST-transferred from libc++ by tools/transfer.py (slug=utilities_optional_optional_nullops_less_equal).
// main -> test_utilities_optional_optional_nullops_less_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <optional>

// template <class T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept;
// template <class T> constexpr bool operator<=(nullopt_t, const optional<T>& x) noexcept;

#include <optional>

#include "test_macros.h"

void test_utilities_optional_optional_nullops_less_equal()
{
    using std::optional;
    using std::nullopt_t;
    using std::nullopt;

    {
    typedef int T;
    typedef optional<T> O;

    constexpr O o1;     // disengaged
    constexpr O o2{1};  // engaged

    static_assert (  (nullopt <= o1), "" );
    static_assert (  (nullopt <= o2), "" );
    static_assert (  (o1 <= nullopt), "" );
    static_assert ( !(o2 <= nullopt), "" );

    static_assert (noexcept(nullopt <= o1), "");
    static_assert (noexcept(o1 <= nullopt), "");
    }

  return;
}
