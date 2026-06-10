// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_ref_convert).
// main -> test_atomics_atomics_ref_convert; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// XFAIL: !has-64-bit-atomics
// XFAIL: !has-1024-bit-atomics

// operator T() const noexcept;

#include <atomic>
#include <cassert>
#include <type_traits>

#include "atomic_helpers.h"
#include "test_helper.h"
#include "test_macros.h"

namespace libcis_ns_atomics_atomics_ref_convert { // libcis: isolate file-scope helpers
template <typename T>
struct TestConvert {
  void operator()() const {
    T x(T(1));

    alignas(std::atomic_ref<T>::required_alignment) T copy = x;
    std::atomic_ref<T> const a(copy);

    T converted = a;
    assert(converted == x);

    ASSERT_NOEXCEPT(T(a));
    static_assert(std::is_nothrow_convertible_v<std::atomic_ref<T>, T>);

    auto store = [](std::atomic_ref<T> const& y, T, T new_val) { y.store(new_val); };
    auto load  = [](std::atomic_ref<T> const& y) { return static_cast<T>(y); };
    test_seq_cst<T>(store, load);
  }
};
} using namespace libcis_ns_atomics_atomics_ref_convert; // libcis


void test_atomics_atomics_ref_convert() {
  TestEachAtomicType<TestConvert>()();
  return;
}
