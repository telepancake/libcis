// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_compare_exchange_strong).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_compare_exchange_strong; file-scope helpers isolated in anon namespace.
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
//     bool
//     atomic_compare_exchange_strong(volatile atomic<T>*,
//                                    atomic<T>::value_type*,
//                                    atomic<T>::value_type) noexcept;
//
// template <class T>
//     bool
//     atomic_compare_exchange_strong(atomic<T>*,
//                                    atomic<T>::value_type*,
//                                    atomic<T>::value_type) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_compare_exchange_strong { // libcis: isolate file-scope helpers
template <class T>
struct TestFn {
  void operator()() const {
    {
        typedef std::atomic<T> A;
        T t(T(1));
        A a(t);
        assert(std::atomic_compare_exchange_strong(&a, &t, T(2)) == true);
        assert(a == T(2));
        assert(t == T(1));
        assert(std::atomic_compare_exchange_strong(&a, &t, T(3)) == false);
        assert(a == T(2));
        assert(t == T(2));

        ASSERT_NOEXCEPT(std::atomic_compare_exchange_strong(&a, &t, T(3)));
    }
    {
        typedef std::atomic<T> A;
        T t(T(1));
        volatile A a(t);
        assert(std::atomic_compare_exchange_strong(&a, &t, T(2)) == true);
        assert(a == T(2));
        assert(t == T(1));
        assert(std::atomic_compare_exchange_strong(&a, &t, T(3)) == false);
        assert(a == T(2));
        assert(t == T(2));

        ASSERT_NOEXCEPT(std::atomic_compare_exchange_strong(&a, &t, T(3)));
    }
  }
};
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_compare_exchange_strong; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_compare_exchange_strong()
{
    TestEachAtomicType<TestFn>()();

  return;
}
