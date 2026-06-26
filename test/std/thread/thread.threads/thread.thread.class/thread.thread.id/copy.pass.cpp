// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_id_copy_22ad2a32).
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

// class thread::id

// id(const id&) = default;

#include <thread>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_id_copy_22ad2a32 { // libcis
int main(int, char**)
{
    std::thread::id id0;
    std::thread::id id1 = id0;
    assert(id1 == id0);

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_id_copy_22ad2a32 (libcis)

