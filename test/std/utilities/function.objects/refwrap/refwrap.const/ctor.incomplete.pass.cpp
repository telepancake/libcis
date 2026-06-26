// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_const_ctor_incomplete_ff13ec1c).
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
// reference_wrapper<T>
//
//  where T is an incomplete type (since C++20)


#include <functional>
#include <cassert>

#include "test_macros.h"


namespace libcis_ns_utilities_function_objects_refwrap_refwrap_const_ctor_incomplete_ff13ec1c { // libcis
struct Foo;

Foo& get_foo();

void test() {
    Foo& foo = get_foo();
    std::reference_wrapper<Foo> ref{foo};
    assert(&ref.get() == &foo);
}

struct Foo { };

Foo& get_foo() {
    static Foo foo;
    return foo;
}

int main(int, char**) {
    test();
    return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_refwrap_refwrap_const_ctor_incomplete_ff13ec1c (libcis)

