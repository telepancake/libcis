// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_id_enabled_hashes).
// main -> test_thread_thread_threads_thread_thread_class_thread_thread_id_enabled_hashes; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03

// <thread>

// Test that <thread> provides all of the arithmetic, enum, and pointer
// hash specializations.

#include <functional>
#include <thread>

#include "poisoned_hash_helper.h"

#include "test_macros.h"

void test_thread_thread_threads_thread_thread_class_thread_thread_id_enabled_hashes() {
  test_library_hash_specializations_available();
  {
    test_hash_enabled<std::thread::id>();
  }

  return;
}
