// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_condition_cv_status_56db0464).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <condition_variable>

// enum class cv_status { no_timeout, timeout };

#include <condition_variable>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_thread_condition_cv_status_56db0464 { // libcis
int main(int, char**)
{
    assert(static_cast<int>(std::cv_status::no_timeout) == 0);
    assert(static_cast<int>(std::cv_status::timeout)    == 1);

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_condition_cv_status_56db0464 (libcis)

