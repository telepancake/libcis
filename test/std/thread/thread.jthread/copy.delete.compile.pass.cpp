// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_jthread_copy_delete_107652b3).
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

// jthread(const jthread&) = delete;
// jthread& operator=(const jthread&) = delete;

#include <thread>
#include <type_traits>

namespace libcis_ns_thread_thread_jthread_copy_delete_107652b3 { // libcis
static_assert(!std::is_copy_constructible_v<std::jthread>);
static_assert(!std::is_copy_assignable_v<std::jthread>);
} // libcis_ns_thread_thread_jthread_copy_delete_107652b3 (libcis)

