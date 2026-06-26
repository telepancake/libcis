// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_const_deduct_05c0b2d6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <functional>

// template <class T>
//   reference_wrapper(T&) -> reference_wrapper<T>;

#include <functional>

namespace libcis_ns_utilities_function_objects_refwrap_refwrap_const_deduct_05c0b2d6 { // libcis
int main(int, char**) {
    int i = 0;
    std::reference_wrapper ri(i);
    static_assert(std::is_same_v<decltype(ri), std::reference_wrapper<int>>);
    std::reference_wrapper ri2(ri);
    static_assert(std::is_same_v<decltype(ri2), std::reference_wrapper<int>>);
    const int j = 0;
    std::reference_wrapper rj(j);
    static_assert(std::is_same_v<decltype(rj), std::reference_wrapper<const int>>);
    std::reference_wrapper rj2(rj);
    static_assert(std::is_same_v<decltype(rj2), std::reference_wrapper<const int>>);

    return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_refwrap_refwrap_const_deduct_05c0b2d6 (libcis)

