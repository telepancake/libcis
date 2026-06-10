// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_ref_bitwise_or_assign).
// main -> test_atomics_atomics_ref_bitwise_or_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
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

namespace libcis_ns_atomics_atomics_ref_bitwise_or_assign { // libcis: isolate file-scope helpers
template <typename T>
concept has_bitwise_or_assign = requires { std::declval<T const>() |= std::declval<T>(); };

template < typename T>
struct TestDoesNotHaveBitwiseOrAssign {
  void operator()() const { static_assert(!has_bitwise_or_assign<std::atomic_ref<T>>); }
};

template <typename T>
struct TestBitwiseOrAssign {
  void operator()() const {
    static_assert(std::is_integral_v<T>);

    alignas(std::atomic_ref<T>::required_alignment) T x(T(1));
    std::atomic_ref<T> const a(x);

    std::same_as<T> decltype(auto) y = (a |= T(2));
    assert(y == T(3));
    assert(x == T(3));
    ASSERT_NOEXCEPT(a |= T(0));
  }
};
} using namespace libcis_ns_atomics_atomics_ref_bitwise_or_assign; // libcis


void test_atomics_atomics_ref_bitwise_or_assign() {
  TestEachIntegralType<TestBitwiseOrAssign>()();

  TestEachFloatingPointType<TestDoesNotHaveBitwiseOrAssign>()();

  TestEachPointerType<TestDoesNotHaveBitwiseOrAssign>()();

  TestDoesNotHaveBitwiseOrAssign<bool>()();
  TestDoesNotHaveBitwiseOrAssign<UserAtomicType>()();
  TestDoesNotHaveBitwiseOrAssign<LargeUserAtomicType>()();

  return;
}
