// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_is_error_code_enum).
// main -> test_diagnostics_syserr_is_error_code_enum; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03

// <system_error>

// template <> struct is_error_code_enum<> : public false_type {};

#include <cstddef>
#include <string>
#include <system_error>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_is_error_code_enum { // libcis: isolate file-scope helpers
template <bool Expected, class T>
void
test()
{
    static_assert((std::is_error_code_enum<T>::value == Expected), "");
#if TEST_STD_VER > 14
    static_assert((std::is_error_code_enum_v<T>      == Expected), "");
    ASSERT_SAME_TYPE(decltype(std::is_error_code_enum_v<T>), const bool);
#endif
}

class A {
    A();
    operator std::error_code () const { return std::error_code(); }
};
} using namespace libcis_ns_diagnostics_syserr_is_error_code_enum; // libcis


// Specialize the template for my class
template <>
struct std::is_error_code_enum<A> : public std::true_type {};

void test_diagnostics_syserr_is_error_code_enum()
{
    test<false, void>();
    test<false, int>();
    test<false, std::nullptr_t>();
    test<false, std::string>();

    test<true, A>();

  return;
}
