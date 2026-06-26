// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_filesystems_class_rec_dir_itr_rec_dir_itr_members_equal_110ed710).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-filesystem
// UNSUPPORTED: availability-filesystem-missing

// <filesystem>

// class recursive_directory_iterator

// bool operator==(default_sentinel_t) const noexcept {           // since C++20
//   return *this == recursive_directory_iterator();
// }

#include <cassert>
#include <iterator>

#include <filesystem>
#include "test_comparisons.h"
namespace libcis_ns_input_output_filesystems_class_rec_dir_itr_rec_dir_itr_members_equal_110ed710 { // libcis
namespace fs = std::filesystem;

int main(int, char**) {
  AssertEqualityAreNoexcept<fs::recursive_directory_iterator>();
  AssertEqualityReturnBool<fs::recursive_directory_iterator>();

  fs::recursive_directory_iterator i;
  assert(testEquality(i, std::default_sentinel, true));

  return 0;

    return 0;
}
} // libcis_ns_input_output_filesystems_class_rec_dir_itr_rec_dir_itr_members_equal_110ed710 (libcis)

