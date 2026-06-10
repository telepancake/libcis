// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_add).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_add; file-scope helpers isolated in anon namespace.
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
//     atomic_fetch_add(volatile atomic<T>* obj, atomic<T>::difference_type) noexcept;
//
// template<class T>
//     T
//     atomic_fetch_add(atomic<T>* obj, atomic<T>::difference_type) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_add { // libcis: isolate file-scope helpers
template <class T>
struct TestFn {
  void operator()() const {
    {
        typedef std::atomic<T> A;
        A t(T(1));
        assert(std::atomic_fetch_add(&t, T(2)) == T(1));
        assert(t == T(3));
        ASSERT_NOEXCEPT(std::atomic_fetch_add(&t, 0));
    }
    {
        typedef std::atomic<T> A;
        volatile A t(T(1));
        assert(std::atomic_fetch_add(&t, T(2)) == T(1));
        assert(t == T(3));
        ASSERT_NOEXCEPT(std::atomic_fetch_add(&t, 0));
    }
  }
};

template <class T>
void testp()
{
    {
        typedef std::atomic<T> A;
        typedef typename std::remove_pointer<T>::type X;
        X a[3] = {0};
        A t(&a[0]);
        assert(std::atomic_fetch_add(&t, 2) == &a[0]);
        std::atomic_fetch_add<T>(&t, 0);
        assert(t == &a[2]);
        ASSERT_NOEXCEPT(std::atomic_fetch_add(&t, 0));
    }
    {
        typedef std::atomic<T> A;
        typedef typename std::remove_pointer<T>::type X;
        X a[3] = {0};
        volatile A t(&a[0]);
        assert(std::atomic_fetch_add(&t, 2) == &a[0]);
        std::atomic_fetch_add<T>(&t, 0);
        assert(t == &a[2]);
        ASSERT_NOEXCEPT(std::atomic_fetch_add(&t, 0));
    }
}
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_add; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_add()
{
    TestEachIntegralType<TestFn>()();
    testp<int*>();
    testp<const int*>();

  return;
}
