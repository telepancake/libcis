// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_hash_error_condition).
// main -> test_diagnostics_syserr_syserr_hash_error_condition; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>
//
// template <>
// struct hash<error_condition>;

#include <system_error>
#include <cassert>
#include <cstddef>
#include <functional>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_hash_error_condition { // libcis: isolate file-scope helpers
void
test(int i)
{
    typedef std::error_condition T;
    typedef std::hash<T> H;
#if TEST_STD_VER <= 14
    static_assert((std::is_same<H::argument_type, T>::value), "" );
    static_assert((std::is_same<H::result_type, std::size_t>::value), "" );
#endif
    ASSERT_NOEXCEPT(H()(T()));
    H h;
    T ec(i, std::system_category());
    const std::size_t result = h(ec);
    LIBCPP_ASSERT(result == static_cast<std::size_t>(i));
    ((void)result); // Prevent unused warning
}
} using namespace libcis_ns_diagnostics_syserr_syserr_hash_error_condition; // libcis


void test_diagnostics_syserr_syserr_hash_error_condition()
{
    test(0);
    test(2);
    test(10);

  return;
}
