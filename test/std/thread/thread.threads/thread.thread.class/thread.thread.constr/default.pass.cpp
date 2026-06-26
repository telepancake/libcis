// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_constr_default_1b0185f0).
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

// class thread

// thread();

#include <thread>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_constr_default_1b0185f0 { // libcis
int main(int, char**)
{
    std::thread t;
    assert(t.get_id() == std::thread::id());

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_constr_default_1b0185f0 (libcis)

