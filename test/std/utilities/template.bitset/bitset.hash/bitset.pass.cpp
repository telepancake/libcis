// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_bitset_hash_bitset).
// main -> test_utilities_template_bitset_bitset_hash_bitset; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <functional>

// template <class T>
// struct hash
// {
//     size_t operator()(T val) const;
// };

#include <bitset>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_bitset_hash_bitset { // libcis: isolate file-scope helpers
template <std::size_t N>
void
test()
{
    typedef std::bitset<N> T;
    typedef std::hash<T> H;
#if TEST_STD_VER <= 14
    static_assert((std::is_same<typename H::argument_type, T>::value), "" );
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "" );
#endif
    ASSERT_NOEXCEPT(H()(T()));

    H h;
    T bs(static_cast<unsigned long long>(N));
    const std::size_t result = h(bs);
    LIBCPP_ASSERT(result == N);
    ((void)result); // Prevent unused warning
}
} using namespace libcis_ns_utilities_template_bitset_bitset_hash_bitset; // libcis


void test_utilities_template_bitset_bitset_hash_bitset()
{
    test<0>();
    test<10>();
    test<100>();
    test<1000>();

  return;
}
