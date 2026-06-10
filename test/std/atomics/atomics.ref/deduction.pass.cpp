// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_ref_deduction).
// main -> test_atomics_atomics_ref_deduction; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_atomics_atomics_ref_deduction { // libcis: isolate file-scope helpers
template <typename T>
struct TestDeduction {
  void operator()() const {
    alignas(std::atomic_ref<T>::required_alignment) T x(T(0));
    std::atomic_ref a(x);
    ASSERT_SAME_TYPE(decltype(a), std::atomic_ref<T>);
  }
};
} using namespace libcis_ns_atomics_atomics_ref_deduction; // libcis


void test_atomics_atomics_ref_deduction() {
  TestEachAtomicType<TestDeduction>()();
  return;
}
