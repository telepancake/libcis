// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_nonmembers_lt_2c3ace22).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// bool operator<(const error_condition& lhs, const error_condition& rhs);

#include <system_error>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_nonmembers_lt_2c3ace22 { // libcis
int main(int, char**)
{
    {
        const std::error_condition ec1(6, std::generic_category());
        const std::error_condition ec2(7, std::generic_category());
        assert(ec1 < ec2);
    }

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_nonmembers_lt_2c3ace22 (libcis)

