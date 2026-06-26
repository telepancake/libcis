// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_shared_ptr_deduction_55476e09).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>
// UNSUPPORTED: c++03, c++11, c++14

// template<class T> class weak_ptr

// weak_ptr(shared_ptr<T>) -> weak_ptr<T>

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_shared_ptr_deduction_55476e09 { // libcis
struct A {};

int main(int, char**)
{
  std::shared_ptr<A> s(new A);
  auto w = std::weak_ptr(s);
  ASSERT_SAME_TYPE(decltype(w), std::weak_ptr<A>);
  assert(!w.expired());
  assert(w.use_count() == 1);
  assert(w.lock().get() == s.get());

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_shared_ptr_deduction_55476e09 (libcis)

