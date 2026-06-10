// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_sub).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_sub; file-scope helpers isolated in anon namespace.
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
//     atomic_fetch_sub(volatile atomic<T>*, atomic<T>::difference_type) noexcept;
//
// template<class T>
//     T
//     atomic_fetch_sub(atomic<T>*, atomic<T>::difference_type) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_sub { // libcis: isolate file-scope helpers
template <class T>
struct TestFn {
  void operator()() const {
    {
        typedef std::atomic<T> A;
        A t(T(3));
        assert(std::atomic_fetch_sub(&t, T(2)) == T(3));
        assert(t == T(1));
        ASSERT_NOEXCEPT(std::atomic_fetch_sub(&t, 0));
    }
    {
        typedef std::atomic<T> A;
        volatile A t(T(3));
        assert(std::atomic_fetch_sub(&t, T(2)) == T(3));
        assert(t == T(1));
        ASSERT_NOEXCEPT(std::atomic_fetch_sub(&t, 0));
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
        A t(&a[2]);
        assert(std::atomic_fetch_sub(&t, 2) == &a[2]);
        std::atomic_fetch_sub<T>(&t, 0);
        assert(t == &a[0]);
        ASSERT_NOEXCEPT(std::atomic_fetch_sub(&t, 0));
    }
    {
        typedef std::atomic<T> A;
        typedef typename std::remove_pointer<T>::type X;
        X a[3] = {0};
        volatile A t(&a[2]);
        assert(std::atomic_fetch_sub(&t, 2) == &a[2]);
        std::atomic_fetch_sub<T>(&t, 0);
        assert(t == &a[0]);
        ASSERT_NOEXCEPT(std::atomic_fetch_sub(&t, 0));
    }
}
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_sub; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_atomic_fetch_sub()
{
    TestEachIntegralType<TestFn>()();
    testp<int*>();
    testp<const int*>();

  return;
}
