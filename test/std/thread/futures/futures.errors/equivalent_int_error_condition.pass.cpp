// transferred+adapted from libc++ by tools/transfer.py (slug=thread_futures_futures_errors_equivalent_int_error_condition_a79bf7c5).
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

// virtual bool equivalent(int code, const error_condition& condition) const;

#include <future>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_errors_equivalent_int_error_condition_a79bf7c5 { // libcis
int main(int, char**)
{
    const std::error_category& e_cat = std::future_category();
    std::error_condition e_cond = e_cat.default_error_condition(5);
    assert(e_cat.equivalent(5, e_cond));
    assert(!e_cat.equivalent(6, e_cond));

  return 0;

    return 0;
}
} // libcis_ns_thread_futures_futures_errors_equivalent_int_error_condition_a79bf7c5 (libcis)

