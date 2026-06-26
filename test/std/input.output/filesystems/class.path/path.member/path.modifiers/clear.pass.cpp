// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_filesystems_class_path_path_member_path_modifiers_clear_e3e41856).
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

// void clear() noexcept

#include <filesystem>
#include <cassert>
#include <type_traits>

#include "assert_macros.h"
#include "count_new.h"
#include "test_iterators.h"
namespace libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_clear_e3e41856 { // libcis
namespace fs = std::filesystem;

int main(int, char**) {
  using namespace fs;
  {
    path p;
    ASSERT_NOEXCEPT(p.clear());
    ASSERT_SAME_TYPE(void, decltype(p.clear()));
    p.clear();
    assert(p.empty());
  }
  {
    const path p("/foo/bar/baz");
    path p2(p);
    assert(p == p2);
    p2.clear();
    assert(p2.empty());
  }

  return 0;

    return 0;
}
} // libcis_ns_input_output_filesystems_class_path_path_member_path_modifiers_clear_e3e41856 (libcis)

