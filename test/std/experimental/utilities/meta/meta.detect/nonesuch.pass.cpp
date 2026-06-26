// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_utilities_meta_meta_detect_nonesuch_5f5f0e08).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11
// <experimental/type_traits>
//
//  struct nonesuch

#include <experimental/type_traits>
#include <string>

#include "test_macros.h"

namespace libcis_ns_experimental_utilities_meta_meta_detect_nonesuch_5f5f0e08 { // libcis
namespace ex = std::experimental;

struct such {};
void foo(const such &) {}
void foo(const ex::nonesuch &) {}

int main(int, char**) {
    foo({});  // nonesuch is not an aggregate

    return 0;

    return 0;
}
} // libcis_ns_experimental_utilities_meta_meta_detect_nonesuch_5f5f0e08 (libcis)

