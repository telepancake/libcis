// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_load).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_load; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// XFAIL: !has-1024-bit-atomics

// <atomic>

// template <class T>
//     T
//     atomic_load(const volatile atomic<T>*) noexcept;
//
// template <class T>
//     T
//     atomic_load(const atomic<T>*) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_load { // libcis: isolate file-scope helpers
template <class T>
struct TestFn {
  void operator()() const {
    typedef std::atomic<T> A;
    A t(T(1));
    assert(std::atomic_load(&t) == T(1));
    volatile A vt(T(2));
    assert(std::atomic_load(&vt) == T(2));

    ASSERT_NOEXCEPT(std::atomic_load(&t));
    ASSERT_NOEXCEPT(std::atomic_load(&vt));
  }
};
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_load; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_load()
{
    TestEachAtomicType<TestFn>()();

  return;
}
