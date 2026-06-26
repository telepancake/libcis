// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_errors_default_error_condition_6ca14ce4).
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

// const error_category& future_category();

// virtual error_condition default_error_condition(int ev) const;

#include <future>
#include <cassert>
#include <system_error>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_errors_default_error_condition_6ca14ce4 { // libcis
int main(int, char**)
{
    const std::error_category& e_cat = std::future_category();
    std::error_condition e_cond = e_cat.default_error_condition(static_cast<int>(std::errc::not_a_directory));
    assert(e_cond.category() == e_cat);
    assert(e_cond.value() == static_cast<int>(std::errc::not_a_directory));

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_errors_default_error_condition_6ca14ce4 (libcis)

