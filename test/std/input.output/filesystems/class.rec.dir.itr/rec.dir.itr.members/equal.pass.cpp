// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_rec_dir_itr_rec_dir_itr_members_equal).
// main -> test_input_output_filesystems_class_rec_dir_itr_rec_dir_itr_members_equal; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-filesystem

// <filesystem>

// class recursive_directory_iterator

// bool operator==(default_sentinel_t) const noexcept {           // since C++20
//   return *this == recursive_directory_iterator();
// }

#include <cassert>
#include <iterator>

#include <filesystem>
#include "test_comparisons.h"
namespace fs = std::filesystem;

void test_input_output_filesystems_class_rec_dir_itr_rec_dir_itr_members_equal() {
  AssertEqualityAreNoexcept<fs::recursive_directory_iterator>();
  AssertEqualityReturnBool<fs::recursive_directory_iterator>();

  fs::recursive_directory_iterator i;
  assert(testEquality(i, std::default_sentinel, true));

  return;
}
