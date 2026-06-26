// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_this_get_id_1922cf32).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <thread>

// thread::id this_thread::get_id();

#include <thread>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_this_get_id_1922cf32 { // libcis
int main(int, char**)
{
    std::thread::id id = std::this_thread::get_id();
    assert(id != std::thread::id());

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_this_get_id_1922cf32 (libcis)

