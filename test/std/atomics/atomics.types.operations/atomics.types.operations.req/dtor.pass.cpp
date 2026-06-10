// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_operations_atomics_types_operations_req_dtor).
// main -> test_atomics_atomics_types_operations_atomics_types_operations_req_dtor; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_dtor { // libcis: isolate file-scope helpers
template <class Tp>
struct CheckTriviallyDestructible {
  void operator()() const {
    typedef std::atomic<Tp> Atomic;
    static_assert(std::is_trivially_destructible<Atomic>::value, "");
  }
};
} using namespace libcis_ns_atomics_atomics_types_operations_atomics_types_operations_req_dtor; // libcis


void test_atomics_atomics_types_operations_atomics_types_operations_req_dtor() {
  TestEachIntegralType<CheckTriviallyDestructible>()();
  TestEachFloatingPointType<CheckTriviallyDestructible>()();
  TestEachPointerType<CheckTriviallyDestructible>()();

  return;
}
