// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_dynamic_ptr_launder_launder).
// main -> test_language_support_support_dynamic_ptr_launder_launder; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <new>

// template <class T> constexpr T* launder(T* p) noexcept;

// UNSUPPORTED: c++03, c++11, c++14

#include <cassert>
#include <new>
#include <type_traits>

namespace libcis_ns_language_support_support_dynamic_ptr_launder_launder { // libcis: isolate file-scope helpers
constexpr int gi = 5;
constexpr float gf = 8.f;
} using namespace libcis_ns_language_support_support_dynamic_ptr_launder_launder; // libcis


void test_language_support_support_dynamic_ptr_launder_launder() {
    static_assert(std::launder(&gi) == &gi, "" );
    static_assert(std::launder(&gf) == &gf, "" );

    const int *i = &gi;
    const float *f = &gf;
    static_assert(std::is_same<decltype(i), decltype(std::launder(i))>::value, "");
    static_assert(std::is_same<decltype(f), decltype(std::launder(f))>::value, "");

    assert(std::launder(i) == i);
    assert(std::launder(f) == f);

  return;
}
