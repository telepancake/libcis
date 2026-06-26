// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_refwrap_refwrap_const_copy_ctor_d94980d9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <functional>

// reference_wrapper

// reference_wrapper(const reference_wrapper<T>& x);

#include <functional>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_refwrap_refwrap_const_copy_ctor_d94980d9 { // libcis
class functor1
{
};

template <class T>
void
test(T& t)
{
    std::reference_wrapper<T> r(t);
    std::reference_wrapper<T> r2 = r;
    assert(&r2.get() == &t);
}

void f() {}

int main(int, char**)
{
    void (*fp)() = f;
    test(fp);
    test(f);
    functor1 f1;
    test(f1);
    int i = 0;
    test(i);
    const int j = 0;
    test(j);

  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_refwrap_refwrap_const_copy_ctor_d94980d9 (libcis)

