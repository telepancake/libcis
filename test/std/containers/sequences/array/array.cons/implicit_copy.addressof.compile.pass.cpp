// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_array_array_cons_implicit_copy_addressof_78b8d1e8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// implicitly generated array assignment operators

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <array>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_array_array_cons_implicit_copy_addressof_78b8d1e8 { // libcis
void test() {
  std::array<operator_hijacker, 1> ao;
  std::array<operator_hijacker, 1> a;
  a = ao;
}
} // libcis_ns_containers_sequences_array_array_cons_implicit_copy_addressof_78b8d1e8 (libcis)

