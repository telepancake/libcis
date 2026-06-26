// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_construct_copy_74c959ae).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// UNSUPPORTED: availability-filesystem-missing

// <filesystem>

// class path

// path(path const&)

#include <filesystem>
#include <cassert>
#include <string>
#include <type_traits>

#include "test_macros.h"
namespace libcis_ns_input_output_filesystems_class_path_path_member_path_construct_copy_74c959ae { // libcis
namespace fs = std::filesystem;

int main(int, char**) {
  using namespace fs;
  static_assert(std::is_copy_constructible<path>::value, "");
  static_assert(!std::is_nothrow_copy_constructible<path>::value, "should not be noexcept");
  const std::string s("foo");
  const path p(s);
  path p2(p);
  assert(p.string() == s);
  assert(p2.string() == s);

  return 0;

    return 0;
}
} // libcis_ns_input_output_filesystems_class_path_path_member_path_construct_copy_74c959ae (libcis)

