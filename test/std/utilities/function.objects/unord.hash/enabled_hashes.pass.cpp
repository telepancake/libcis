// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_unord_hash_enabled_hashes).
// main -> test_utilities_function_objects_unord_hash_enabled_hashes; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <functional>

// Test that <functional> provides all of the arithmetic, enum, and pointer
// hash specializations.

#include <functional>

#include "poisoned_hash_helper.h"

#include "test_macros.h"

void test_utilities_function_objects_unord_hash_enabled_hashes() {
  test_library_hash_specializations_available();

  return;
}
