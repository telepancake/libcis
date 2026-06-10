// AST-transferred from libc++ by tools/transfer.py (slug=atomics_atomics_types_generic_trivially_copyable).
// main -> test_atomics_atomics_types_generic_trivially_copyable; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <atomic>

// template <class T>
// struct atomic;

// Make sure atomic<TriviallyCopyable> can be instantiated.

#include <atomic>
#include <new>
#include <cassert>
#include <chrono> // for nanoseconds

#include "test_macros.h"

#ifndef TEST_HAS_NO_THREADS
#  include <thread> // for thread_id
#endif

namespace libcis_ns_atomics_atomics_types_generic_trivially_copyable { // libcis: isolate file-scope helpers
struct TriviallyCopyable {
  explicit TriviallyCopyable(int i) : i_(i) { }
  int i_;
};

template <class T>
void test(T t) {
  std::atomic<T> t0(t);
}
} using namespace libcis_ns_atomics_atomics_types_generic_trivially_copyable; // libcis


void test_atomics_atomics_types_generic_trivially_copyable() {
  test(TriviallyCopyable(42));
  test(std::chrono::nanoseconds(2));
#ifndef TEST_HAS_NO_THREADS
  test(std::this_thread::get_id());
#endif

  return;
}
