// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_hash_enabled_hash_1b0b3830).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <system_error>

// Test that <system_error> provides all of the arithmetic, enum, and pointer
// hash specializations.

#include <system_error>

#include "poisoned_hash_helper.h"

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_hash_enabled_hash_1b0b3830 { // libcis
int main(int, char**) {
  test_library_hash_specializations_available();
  {
    test_hash_enabled<std::error_code>();
    test_hash_enabled<std::error_condition>();
  }

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_hash_enabled_hash_1b0b3830 (libcis)

