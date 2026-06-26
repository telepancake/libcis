// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_array_front_back_const_77302895).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// const_reference front() const; // constexpr in C++14
// const_reference back() const;  // constexpr in C++14

#include <array>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_front_back_const_77302895 { // libcis
TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C const c = {1, 2, 3.5};
        C::const_reference r1 = c.front();
        assert(r1 == 1);

        C::const_reference r2 = c.back();
        assert(r2 == 3.5);
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C const c = {};
        ASSERT_SAME_TYPE(decltype(c.back()), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(c.back());
        ASSERT_SAME_TYPE(decltype(c.front()), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(c.front());
        if (c.size() > (0)) { // always false
            TEST_IGNORE_NODISCARD c.front();
            TEST_IGNORE_NODISCARD c.back();
        }
    }
    {
        typedef double T;
        typedef std::array<const T, 0> C;
        C const c = {};
        ASSERT_SAME_TYPE(decltype(c.back()), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(c.back());
        ASSERT_SAME_TYPE(decltype(c.front()), C::const_reference);
        LIBCPP_ASSERT_NOEXCEPT(c.front());
        if (c.size() > (0)) {
            TEST_IGNORE_NODISCARD c.front();
            TEST_IGNORE_NODISCARD c.back();
        }
    }

    return true;
}

int main(int, char**)
{
    tests();
#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif
    return 0;

    return 0;
}
} // libcis_ns_containers_sequences_array_front_back_const_77302895 (libcis)

