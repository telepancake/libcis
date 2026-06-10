// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_fs_filesystem_synopsis_space_info).
// main -> test_input_output_filesystems_fs_filesystem_synopsis_space_info; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <filesystem>

// friend bool operator==(const space_info&, const space_info&);

#include <filesystem>

#include "test_macros.h"
#include "test_comparisons.h"
namespace fs = std::filesystem;
using namespace fs;

namespace libcis_ns_input_output_filesystems_fs_filesystem_synopsis_space_info { // libcis: isolate file-scope helpers
constexpr bool test() {
  assert(testEquality(space_info{1, 2, 3}, space_info{1, 2, 3}, true));
  assert(testEquality(space_info{0, 2, 3}, space_info{1, 2, 3}, false));
  assert(testEquality(space_info{1, 0, 3}, space_info{1, 2, 3}, false));
  assert(testEquality(space_info{1, 2, 0}, space_info{1, 2, 3}, false));

  return true;
}
} using namespace libcis_ns_input_output_filesystems_fs_filesystem_synopsis_space_info; // libcis


void test_input_output_filesystems_fs_filesystem_synopsis_space_info() {
  using space_info = std::filesystem::space_info;

  AssertEqualityAreNoexcept<space_info>();
  AssertEqualityReturnBool<space_info>();

  test();
  static_assert(test());

  return;
}
