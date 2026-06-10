// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_meta_meta_detect_nonesuch).
// main -> test_experimental_utilities_meta_meta_detect_nonesuch; file-scope helpers isolated in anon namespace.
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

namespace ex = std::experimental;

namespace libcis_ns_experimental_utilities_meta_meta_detect_nonesuch { // libcis: isolate file-scope helpers
struct such {};
void foo(const such &) {}
void foo(const ex::nonesuch &) {}
} using namespace libcis_ns_experimental_utilities_meta_meta_detect_nonesuch; // libcis


void test_experimental_utilities_meta_meta_detect_nonesuch() {
    foo({});  // nonesuch is not an aggregate

    return;
}
