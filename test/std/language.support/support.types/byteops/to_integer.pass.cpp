// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_types_byteops_to_integer_62dc9608).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <type_traits>
#include <test_macros.h>

// UNSUPPORTED: c++03, c++11, c++14

// template <class IntegerType>
//    constexpr IntegerType to_integer(byte b) noexcept;
// This function shall not participate in overload resolution unless
//   is_integral_v<IntegerType> is true.

namespace libcis_ns_language_support_support_types_byteops_to_integer_62dc9608 { // libcis
int main(int, char**) {
    constexpr std::byte b1{static_cast<std::byte>(1)};
    constexpr std::byte b3{static_cast<std::byte>(3)};

    static_assert(noexcept(std::to_integer<int>(b1)), "" );
    static_assert(std::is_same<int, decltype(std::to_integer<int>(b1))>::value, "" );
    static_assert(std::is_same<long, decltype(std::to_integer<long>(b1))>::value, "" );
    static_assert(std::is_same<unsigned short, decltype(std::to_integer<unsigned short>(b1))>::value, "" );

    static_assert(std::to_integer<int>(b1) == 1, "");
    static_assert(std::to_integer<int>(b3) == 3, "");

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_types_byteops_to_integer_62dc9608 (libcis)

