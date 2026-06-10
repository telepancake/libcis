// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_class_ctor_default).
// main -> test_thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_class_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// class mutex;

// mutex() noexcept;

#include <mutex>
#include <cassert>
#include <type_traits>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_class_ctor_default { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_default_constructible<std::mutex>::value, "");
} using namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_class_ctor_default; // libcis


void test_thread_thread_mutex_thread_mutex_requirements_thread_mutex_requirements_mutex_thread_mutex_class_ctor_default() {
  // The mutex is unlocked after default construction
  {
    std::mutex m;
    assert(m.try_lock());
    m.unlock();
  }

  return;
}
