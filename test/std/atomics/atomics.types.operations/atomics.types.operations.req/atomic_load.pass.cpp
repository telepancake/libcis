// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_atomic_load_4f0e6111).
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

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_load_4f0e6111 { // libcis
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

int main(int, char**)
{
    TestEachAtomicType<TestFn>()();

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_atomic_load_4f0e6111 (libcis)

