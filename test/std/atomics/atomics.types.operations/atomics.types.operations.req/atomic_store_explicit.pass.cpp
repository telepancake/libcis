// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_store_explicit_641dbbdc).
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
//     void
//     atomic_store_explicit(volatile atomic<T>*, atomic<T>::value_type,
//                           memory_order) noexcept;
//
// template <class T>
//     void
//     atomic_store_explicit(atomic<T>*, atomic<T>::value_type,
//                           memory_order) noexcept;

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_store_explicit_641dbbdc { // libcis
template <class T>
struct TestFn {
  void operator()() const {
    typedef std::atomic<T> A;
    A t;
    std::atomic_store_explicit(&t, T(1), std::memory_order_seq_cst);
    assert(t == T(1));
    volatile A vt;
    std::atomic_store_explicit(&vt, T(2), std::memory_order_seq_cst);
    assert(vt == T(2));

    ASSERT_NOEXCEPT(std::atomic_store_explicit(&t, T(1), std::memory_order_seq_cst));
    ASSERT_NOEXCEPT(std::atomic_store_explicit(&vt, T(2), std::memory_order_seq_cst));
  }
};


int main(int, char**)
{
    TestEachAtomicType<TestFn>()();

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_store_explicit_641dbbdc (libcis)

