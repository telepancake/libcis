// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default_56b0f235).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr();

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default_56b0f235 { // libcis
struct A {};

template <class T>
void test() {
  std::shared_ptr<T> p;
  assert(p.use_count() == 0);
  assert(p.get() == 0);
}

int main(int, char**) {
  test<int>();
  test<int const>();
  test<A>();
  test<A const>();
  test<int*>();
  test<int const*>();
  test<int[]>();
  test<int[8]>();

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default_56b0f235 (libcis)

