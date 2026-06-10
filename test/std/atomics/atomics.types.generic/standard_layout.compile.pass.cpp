// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_generic_standard_layout).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
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

namespace libcis_ns_atomics_atomics_types_generic_standard_layout { // libcis: isolate file-scope helpers
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
} using namespace libcis_ns_atomics_atomics_types_generic_standard_layout; // libcis

