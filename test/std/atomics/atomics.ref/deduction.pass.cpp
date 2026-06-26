// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_ref_deduction_83ec1a79).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <atomic>

// explicit atomic_ref(T&);

#include <atomic>
#include <type_traits>

#include "atomic_helpers.h"
#include "test_macros.h"

namespace libcis_ns_atomics_atomics_ref_deduction_83ec1a79 { // libcis
template <typename T>
struct TestDeduction {
  void operator()() const {
    T x(T(0));
    std::atomic_ref a(x);
    ASSERT_SAME_TYPE(decltype(a), std::atomic_ref<T>);
  }
};

int main(int, char**) {
  TestEachAtomicType<TestDeduction>()();
  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_ref_deduction_83ec1a79 (libcis)

