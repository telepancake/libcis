// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_dtor_ca2ed9f0).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <atomic>

// constexpr atomic<T>::~atomic()

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_dtor_ca2ed9f0 { // libcis
template <class Tp>
struct CheckTriviallyDestructible {
  void operator()() const {
    typedef std::atomic<Tp> Atomic;
    static_assert(std::is_trivially_destructible<Atomic>::value, "");
  }
};

int main(int, char**) {
  TestEachIntegralType<CheckTriviallyDestructible>()();
  TestEachFloatingPointType<CheckTriviallyDestructible>()();
  TestEachPointerType<CheckTriviallyDestructible>()();

  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_dtor_ca2ed9f0 (libcis)

