// AST-transferred from libc++ by tools/transfer.py (slug=input_output_filesystems_fs_enum_enum_directory_options).
// main -> test_input_output_filesystems_fs_enum_enum_directory_options; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <filesystem>

// enum class directory_options;

#include <filesystem>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "check_bitmask_types.h"
namespace fs = std::filesystem;

namespace libcis_ns_input_output_filesystems_fs_enum_enum_directory_options { // libcis: isolate file-scope helpers
constexpr fs::directory_options ME(int val) { return static_cast<fs::directory_options>(val); }
} using namespace libcis_ns_input_output_filesystems_fs_enum_enum_directory_options; // libcis


void test_input_output_filesystems_fs_enum_enum_directory_options() {
  typedef fs::directory_options E;
  static_assert(std::is_enum<E>::value, "");

  // Check that E is a scoped enum by checking for conversions.
  typedef std::underlying_type<E>::type UT;
  static_assert(!std::is_convertible<E, UT>::value, "");
  LIBCPP_STATIC_ASSERT(std::is_same<UT, unsigned char>::value, "");

  typedef check_bitmask_type<E, E::follow_directory_symlink, E::skip_permission_denied> BitmaskTester;
  assert(BitmaskTester::check());

  static_assert(
        E::none                     == ME(0) &&
        E::follow_directory_symlink == ME(1) &&
        E::skip_permission_denied   == ME(2),
        "Expected enumeration values do not match");


  return;
}
