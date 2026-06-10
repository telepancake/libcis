// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_hash_enabled).
// main -> test_input_output_filesystems_class_path_path_member_path_hash_enabled; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <filesystem>

// Test that <filesystem> provides all of the arithmetic, enum, and pointer
// hash specializations.

#include <filesystem>
#include "poisoned_hash_helper.h"
namespace fs = std::filesystem;

void test_input_output_filesystems_class_path_path_member_path_hash_enabled() {
  test_library_hash_specializations_available();
  test_hash_enabled<fs::path>();

  return;
}
