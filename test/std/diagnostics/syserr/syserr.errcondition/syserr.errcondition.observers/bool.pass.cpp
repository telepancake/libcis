// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_observers_bool_04491556).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// explicit operator bool() const;

#include <system_error>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_observers_bool_04491556 { // libcis
int main(int, char**)
{
    {
        const std::error_condition ec(6, std::generic_category());
        assert(static_cast<bool>(ec));
    }
    {
        const std::error_condition ec(0, std::generic_category());
        assert(!static_cast<bool>(ec));
    }

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_observers_bool_04491556 (libcis)

