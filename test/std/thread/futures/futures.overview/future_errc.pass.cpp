// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_overview_future_errc_335ae7ea).
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
// Libc++'s enum class emulation does not allow static_cast<Enum>(0) to work.

// <future>

// enum class future_errc
// {
//     broken_promise = implementation-defined,
//     future_already_retrieved = implementation-defined,
//     promise_already_satisfied = implementation-defined,
//     no_state = implementation-defined
// };

#include <future>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_overview_future_errc_335ae7ea { // libcis
int main(int, char**)
{
    static_assert(std::future_errc::broken_promise != std::future_errc::future_already_retrieved, "");
    static_assert(std::future_errc::broken_promise != std::future_errc::promise_already_satisfied, "");
    static_assert(std::future_errc::broken_promise != std::future_errc::no_state, "");
    static_assert(std::future_errc::future_already_retrieved != std::future_errc::promise_already_satisfied, "");
    static_assert(std::future_errc::future_already_retrieved != std::future_errc::no_state, "");
    static_assert(std::future_errc::promise_already_satisfied != std::future_errc::no_state, "");

    static_assert(std::future_errc::broken_promise != static_cast<std::future_errc>(0), "");
    static_assert(std::future_errc::future_already_retrieved != static_cast<std::future_errc>(0), "");
    static_assert(std::future_errc::promise_already_satisfied != static_cast<std::future_errc>(0), "");
    static_assert(std::future_errc::no_state != static_cast<std::future_errc>(0), "");

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_overview_future_errc_335ae7ea (libcis)

