// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_hardware_inference_size_030068c3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// UNSUPPORTED: (clang || apple-clang) && stdlib=libc++

#include <new>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_hardware_inference_size_030068c3 { // libcis
ASSERT_SAME_TYPE(decltype(std::hardware_destructive_interference_size), const std::size_t);
ASSERT_SAME_TYPE(decltype(std::hardware_constructive_interference_size), const std::size_t);
} // libcis_ns_language_support_support_dynamic_hardware_inference_size_030068c3 (libcis)

