// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_init).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_init; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// XFAIL: !has-1024-bit-atomics
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <atomic>

// template <class T>
//     void
//     atomic_init(volatile atomic<T>* obj, atomic<T>::value_type desr) noexcept;
//
// template <class T>
//     void
//     atomic_init(atomic<T>* obj, atomic<T>::value_type desr) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_init { // libcis: isolate file-scope helpers
template <class T>
struct TestFn {
  void operator()() const {
    typedef std::atomic<T> A;
    A t;
    std::atomic_init(&t, T(1));
    assert(t == T(1));
    volatile A vt;
    std::atomic_init(&vt, T(2));
    assert(vt == T(2));

    ASSERT_NOEXCEPT(std::atomic_init(&t, T(1)));
    ASSERT_NOEXCEPT(std::atomic_init(&vt, T(2)));
  }
};
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_init; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_init()
{
    TestEachAtomicType<TestFn>()();

  return;
}
