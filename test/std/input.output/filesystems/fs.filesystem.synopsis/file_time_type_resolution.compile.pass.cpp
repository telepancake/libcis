// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_filesystems_fs_filesystem_synopsis_file_time_type_resolution_387f5282).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, windows
// UNSUPPORTED: availability-filesystem-missing

// MS STL and libstdc++ use the native windows file timestamp resolution,
// with 100 ns resolution.

// <filesystem>

// typedef TrivialClock file_time_type;

#include <filesystem>
#include <chrono>
#include <ratio>
#include <type_traits>

#include "test_macros.h"
namespace libcis_ns_input_output_filesystems_fs_filesystem_synopsis_file_time_type_resolution_387f5282 { // libcis
namespace fs = std::filesystem;
using namespace fs;
using Dur = file_time_type::duration;
using Period = Dur::period;
ASSERT_SAME_TYPE(Period, std::nano);
} // libcis_ns_input_output_filesystems_fs_filesystem_synopsis_file_time_type_resolution_387f5282 (libcis)

