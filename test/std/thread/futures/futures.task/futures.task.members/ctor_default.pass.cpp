// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_task_futures_task_members_ctor_default_80d6885a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03

// <future>

// class packaged_task<R(ArgTypes...)>

// packaged_task();

#include <future>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_task_futures_task_members_ctor_default_80d6885a { // libcis
struct A {};

int main(int, char**)
{
    std::packaged_task<A(int, char)> p;
    assert(!p.valid());

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_task_futures_task_members_ctor_default_80d6885a (libcis)

