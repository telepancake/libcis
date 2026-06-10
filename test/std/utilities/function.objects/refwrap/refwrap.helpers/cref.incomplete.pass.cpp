// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_helpers_cref_incomplete).
// main -> test_utilities_function_objects_refwrap_refwrap_helpers_cref_incomplete; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <functional>
//
// reference_wrapper
//
// template <ObjectType T> reference_wrapper<const T> cref(const T& t);
//
//  where T is an incomplete type (since C++20)

#include <functional>
#include <cassert>

#include "test_macros.h"


namespace libcis_ns_utilities_function_objects_refwrap_refwrap_helpers_cref_incomplete { // libcis: isolate file-scope helpers
struct Foo;

Foo& get_foo();

void test() {
    Foo const& foo = get_foo();
    std::reference_wrapper<Foo const> ref = std::cref(foo);
    assert(&ref.get() == &foo);
}

struct Foo { };

Foo& get_foo() {
    static Foo foo;
    return foo;
}
} using namespace libcis_ns_utilities_function_objects_refwrap_refwrap_helpers_cref_incomplete; // libcis


void test_utilities_function_objects_refwrap_refwrap_helpers_cref_incomplete() {
    test();
    return;
}
