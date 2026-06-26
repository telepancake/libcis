// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_overview_future_status_54c16147).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <future>

// enum class future_status
// {
//     ready,
//     timeout,
//     deferred
// };

#include <future>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_overview_future_status_54c16147 { // libcis
int main(int, char**)
{
    static_assert(static_cast<int>(std::future_status::ready) == 0, "");
    static_assert(static_cast<int>(std::future_status::timeout) == 1, "");
    static_assert(static_cast<int>(std::future_status::deferred) == 2, "");

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_overview_future_status_54c16147 (libcis)

