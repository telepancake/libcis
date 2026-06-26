// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_once_thread_once_onceflag_default_072b0179).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// struct once_flag;

// constexpr once_flag() noexcept;

#include <mutex>
#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_once_thread_once_onceflag_default_072b0179 { // libcis
int main(int, char**)
{
    {
    std::once_flag f;
    (void)f;
    }
#if TEST_STD_VER >= 11
    {
    constexpr std::once_flag f;
    (void)f;
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_once_thread_once_onceflag_default_072b0179 (libcis)

