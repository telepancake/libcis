// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_types_byteops_rshift_assign).
// main -> test_language_support_support_types_byteops_rshift_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <test_macros.h>

// UNSUPPORTED: c++03, c++11, c++14

// template <class IntegerType>
//   constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept;
// This function shall not participate in overload resolution unless
//   is_integral_v<IntegerType> is true.


namespace libcis_ns_language_support_support_types_byteops_rshift_assign { // libcis: isolate file-scope helpers
constexpr std::byte test(std::byte b) {
    return b >>= 2;
    }
} using namespace libcis_ns_language_support_support_types_byteops_rshift_assign; // libcis



void test_language_support_support_types_byteops_rshift_assign() {
    std::byte b;  // not constexpr, just used in noexcept check
    constexpr std::byte b16{static_cast<std::byte>(16)};
    constexpr std::byte b192{static_cast<std::byte>(192)};

    static_assert(noexcept(b >>= 2), "" );

    static_assert(std::to_integer<int>(test(b16))  ==  4, "" );
    static_assert(std::to_integer<int>(test(b192)) == 48, "" );

  return;
}
