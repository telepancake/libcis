// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_construct_copy).
// main -> test_input_output_filesystems_class_path_path_member_path_construct_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <filesystem>

// class path

// path(path const&)

#include <filesystem>
#include <cassert>
#include <string>
#include <type_traits>

#include "test_macros.h"
namespace fs = std::filesystem;

void test_input_output_filesystems_class_path_path_member_path_construct_copy() {
  using namespace fs;
  static_assert(std::is_copy_constructible<path>::value, "");
  static_assert(!std::is_nothrow_copy_constructible<path>::value, "should not be noexcept");
  const std::string s("foo");
  const path p(s);
  path p2(p);
  assert(p.string() == s);
  assert(p2.string() == s);

  return;
}
