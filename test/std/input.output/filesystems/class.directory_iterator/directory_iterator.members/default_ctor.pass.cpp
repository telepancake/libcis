// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_filesystems_class_directory_iterator_directory_iterator_members_default_ctor_39f180b8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// UNSUPPORTED: no-filesystem
// UNSUPPORTED: availability-filesystem-missing

// <filesystem>

// class directory_iterator

// directory_iterator::directory_iterator() noexcept


#include <filesystem>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
namespace libcis_ns_input_output_filesystems_class_directory_iterator_directory_iterator_members_default_ctor_39f180b8 { // libcis
namespace fs = std::filesystem;

int main(int, char**) {
    {
        static_assert(std::is_nothrow_default_constructible<fs::directory_iterator>::value, "");
    }
    {
        fs::directory_iterator d1;
        const fs::directory_iterator d2;
        assert(d1 == d2);
    }

  return 0;

    return 0;
}
} // libcis_ns_input_output_filesystems_class_directory_iterator_directory_iterator_members_default_ctor_39f180b8 (libcis)

