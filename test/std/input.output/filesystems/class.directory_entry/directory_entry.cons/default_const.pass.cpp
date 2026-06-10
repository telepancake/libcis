// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_class_directory_entry_directory_entry_cons_default_const).
// main -> test_input_output_filesystems_class_directory_entry_directory_entry_cons_default_const; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <filesystem>

// class directory_entry

//          directory_entry() noexcept = default;

#include <filesystem>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
namespace fs = std::filesystem;

void test_input_output_filesystems_class_directory_entry_directory_entry_cons_default_const() {
  using namespace fs;
  // Default
  {
    static_assert(std::is_nothrow_default_constructible<directory_entry>::value,
                  "directory_entry must have a nothrow default constructor");
    const directory_entry e;
    assert(e.path() == path());
  }

  return;
}
