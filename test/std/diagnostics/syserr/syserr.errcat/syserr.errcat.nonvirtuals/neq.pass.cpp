// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcat_syserr_errcat_nonvirtuals_neq_d788d398).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_category

// bool operator!=(const error_category& rhs) const;

#include <system_error>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcat_syserr_errcat_nonvirtuals_neq_d788d398 { // libcis
int main(int, char**)
{
    const std::error_category& e_cat1 = std::generic_category();
    const std::error_category& e_cat2 = std::generic_category();
    const std::error_category& e_cat3 = std::system_category();
    assert(!(e_cat1 != e_cat2));
    assert(e_cat1 != e_cat3);

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcat_syserr_errcat_nonvirtuals_neq_d788d398 (libcis)

