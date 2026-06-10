// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default { // libcis: isolate file-scope helpers
struct A {};

template <class T>
void test() {
  std::shared_ptr<T> p;
  assert(p.use_count() == 0);
  assert(p.get() == 0);
}
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default; // libcis


void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_default() {
  test<int>();
  test<int const>();
  test<A>();
  test<A const>();
  test<int*>();
  test<int const*>();
  test<int[]>();
  test<int[8]>();

  return;
}
