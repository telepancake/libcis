// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_guard_implicit_ctad_938ab3c1).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <mutex>

// template <class Mutex> class lock_guard;

// Make sure that the implicitly-generated CTAD works.

#include <mutex>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_implicit_ctad_938ab3c1 { // libcis
int main(int, char**) {
  MyMutex m;
  {
    std::lock_guard lg(m);
    ASSERT_SAME_TYPE(decltype(lg), std::lock_guard<MyMutex>);
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_implicit_ctad_938ab3c1 (libcis)


