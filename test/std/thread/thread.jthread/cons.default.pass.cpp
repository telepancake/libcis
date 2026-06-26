// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_jthread_cons_default_72919d85).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14, c++17
// XFAIL: availability-synchronization_library-missing

// jthread() noexcept;

#include <cassert>
#include <stop_token>
#include <thread>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_cons_default_72919d85 { // libcis
static_assert(std::is_nothrow_default_constructible_v<std::jthread>);

int main(int, char**) {
  {
    std::jthread jt = {}; // implicit
    assert(!jt.get_stop_source().stop_possible());
    assert(jt.get_id() == std::jthread::id());
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_jthread_cons_default_72919d85 (libcis)

