// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcode_syserr_errcode_modifiers_assign_3cf3124a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_code

// void assign(int val, const error_category& cat);

#include <system_error>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_modifiers_assign_3cf3124a { // libcis
int main(int, char**)
{
    {
        std::error_code ec;
        ec.assign(6, std::system_category());
        assert(ec.value() == 6);
        assert(ec.category() == std::system_category());
    }
    {
        std::error_code ec;
        ec.assign(8, std::generic_category());
        assert(ec.value() == 8);
        assert(ec.category() == std::generic_category());
    }

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_modifiers_assign_3cf3124a (libcis)

