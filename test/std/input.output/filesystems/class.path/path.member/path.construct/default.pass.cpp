// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_construct_default).
// main -> test_input_output_filesystems_class_path_path_member_path_construct_default; file-scope helpers isolated in anon namespace.
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

// path() noexcept

#include <filesystem>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
namespace fs = std::filesystem;

void test_input_output_filesystems_class_path_path_member_path_construct_default() {
  using namespace fs;
  static_assert(std::is_nothrow_default_constructible<path>::value, "");
  const path p;
  assert(p.empty());

  return;
}
