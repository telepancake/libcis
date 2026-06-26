// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_types_generic_standard_layout_0b605ea0).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <atomic>

#include <atomic>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "atomic_helpers.h"

namespace libcis_ns_atomics_atomics_types_generic_standard_layout_0b605ea0 { // libcis
template <class Tp>
struct CheckStandardLayout {
  void operator()() const {
    typedef std::atomic<Tp> Atomic;
    static_assert(std::is_standard_layout<Atomic>::value, "");
  }
};

void tests() {
  TestEachIntegralType<CheckStandardLayout>()();
  TestEachFloatingPointType<CheckStandardLayout>()();
  TestEachPointerType<CheckStandardLayout>()();
}
} // libcis_ns_atomics_atomics_types_generic_standard_layout_0b605ea0 (libcis)

