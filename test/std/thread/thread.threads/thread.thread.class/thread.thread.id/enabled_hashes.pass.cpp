// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_id_enabled_hashes_05b6a1dc).
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

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_id_enabled_hashes_05b6a1dc { // libcis
int main(int, char**) {
  test_library_hash_specializations_available();
  {
    test_hash_enabled<std::thread::id>();
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_id_enabled_hashes_05b6a1dc (libcis)

