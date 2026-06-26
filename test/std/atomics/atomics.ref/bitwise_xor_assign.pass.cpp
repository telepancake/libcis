// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_ref_bitwise_xor_assign_a4e00d82).
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// XFAIL: !has-64-bit-atomics

// integral-type operator|=(integral-type) const noexcept;

#include <atomic>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <utility>

#include "atomic_helpers.h"
#include "test_macros.h"

namespace libcis_ns_atomics_atomics_ref_bitwise_xor_assign_a4e00d82 { // libcis
template <typename T>
concept has_bitwise_xor_assign = requires { std::declval<T const>() ^= std::declval<T>(); };

template <typename T>
struct TestDoesNotHaveBitwiseXorAssign {
  void operator()() const { static_assert(!has_bitwise_xor_assign<std::atomic_ref<float>>); }
};

template <typename T>
struct TestBitwiseXorAssign {
  void operator()() const {
    static_assert(std::is_integral_v<T>);

    T x(T(1));
    std::atomic_ref<T> const a(x);

    std::same_as<T> decltype(auto) y = (a ^= T(2));
    assert(y == T(3));
    assert(x == T(3));
    ASSERT_NOEXCEPT(a ^= T(0));
  }
};

int main(int, char**) {
  TestEachIntegralType<TestBitwiseXorAssign>()();

  TestEachFloatingPointType<TestDoesNotHaveBitwiseXorAssign>()();

  TestEachPointerType<TestDoesNotHaveBitwiseXorAssign>()();

  TestDoesNotHaveBitwiseXorAssign<bool>()();
  TestDoesNotHaveBitwiseXorAssign<UserAtomicType>()();
  TestDoesNotHaveBitwiseXorAssign<LargeUserAtomicType>()();

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_ref_bitwise_xor_assign_a4e00d82 (libcis)

