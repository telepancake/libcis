// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_or).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_or; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// XFAIL: !has-64-bit-atomics

// <atomic>

// template<class T>
//     T
//     atomic_fetch_or(volatile atomic<T>*, atomic<T>::value_type) noexcept;
//
// template<class T>
//     T
//     atomic_fetch_or(atomic<T>*, atomic<T>::value_type) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_or { // libcis: isolate file-scope helpers
template <class T>
struct TestFn {
  void operator()() const {
    {
        typedef std::atomic<T> A;
        A t(T(1));
        assert(std::atomic_fetch_or(&t, T(2)) == T(1));
        assert(t == T(3));

        ASSERT_NOEXCEPT(std::atomic_fetch_or(&t, T(2)));
    }
    {
        typedef std::atomic<T> A;
        volatile A t(T(3));
        assert(std::atomic_fetch_or(&t, T(2)) == T(3));
        assert(t == T(3));

        ASSERT_NOEXCEPT(std::atomic_fetch_or(&t, T(2)));
    }
  }
};
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_or; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_or()
{
    TestEachIntegralType<TestFn>()();

  return;
}
