// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_int_error_category_19447ad8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_code

// error_code(int val, const error_category& cat);

#include <system_error>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_int_error_category_19447ad8 { // libcis
int main(int, char**)
{
    {
        std::error_code ec(6, std::system_category());
        assert(ec.value() == 6);
        assert(ec.category() == std::system_category());
    }
    {
        std::error_code ec(8, std::generic_category());
        assert(ec.value() == 8);
        assert(ec.category() == std::generic_category());
    }

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_int_error_category_19447ad8 (libcis)

