// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_types_byteops_lshift_assign_0fa4e374).
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
//   constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept;
// This function shall not participate in overload resolution unless
//   is_integral_v<IntegerType> is true.


namespace libcis_ns_language_support_support_types_byteops_lshift_assign_0fa4e374 { // libcis
constexpr std::byte test(std::byte b) {
    return b <<= 2;
    }


int main(int, char**) {
    std::byte b;  // not constexpr, just used in noexcept check
    constexpr std::byte b2{static_cast<std::byte>(2)};
    constexpr std::byte b3{static_cast<std::byte>(3)};

    static_assert(noexcept(b <<= 2), "" );

    static_assert(std::to_integer<int>(test(b2)) ==  8, "" );
    static_assert(std::to_integer<int>(test(b3)) == 12, "" );


  return 0;

    return 0;
}
} // libcis_ns_language_support_support_types_byteops_lshift_assign_0fa4e374 (libcis)

