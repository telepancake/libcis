// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_constructors_default_639868af).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// error_condition();

#include <system_error>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_constructors_default_639868af { // libcis
int main(int, char**)
{
    std::error_condition ec;
    assert(ec.value() == 0);
    assert(ec.category() == std::generic_category());

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_constructors_default_639868af (libcis)

