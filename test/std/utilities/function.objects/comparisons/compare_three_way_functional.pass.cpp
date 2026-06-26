// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_comparisons_compare_three_way_functional_e1fa3ed8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <functional>

// Test that std::compare_three_way is defined in <functional>,
// not only in <compare>.

#include <functional>
#include <cassert>

namespace libcis_ns_utilities_function_objects_comparisons_compare_three_way_functional_e1fa3ed8 { // libcis
int main(int, char**)
{
    assert(std::compare_three_way()(1, 2) < 0);
    assert(std::compare_three_way()(1, 1) == 0);
    assert(std::compare_three_way()(2, 1) > 0);

    return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_comparisons_compare_three_way_functional_e1fa3ed8 (libcis)

