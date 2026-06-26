// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_comparisons_transparent_three_way_ebb2d062).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <functional>

namespace libcis_ns_utilities_function_objects_comparisons_transparent_three_way_ebb2d062 { // libcis
template<class T>
concept Transparent = requires {
    typename T::is_transparent;
};

static_assert(Transparent<std::compare_three_way>);
} // libcis_ns_utilities_function_objects_comparisons_transparent_three_way_ebb2d062 (libcis)

