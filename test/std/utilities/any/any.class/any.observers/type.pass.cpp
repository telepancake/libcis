// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_any_any_class_any_observers_type_764b237f).
// libcis: adapted for target ({'rtti': 4}).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: no-rtti

// <any>

// any::type() noexcept

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"

namespace libcis_ns_utilities_any_any_class_any_observers_type_764b237f { // libcis
int main(int, char**)
{
    {
        const std::any a;
        /* libcis: removed (rtti: untestable on target) */
        ASSERT_NOEXCEPT(a.type());
    }
    {
        std::any a = small(1);
        /* libcis: removed (rtti: untestable on target) */
    }
    {
        std::any a = large(1);
        /* libcis: removed (rtti: untestable on target) */
    }
    {
        int arr[3];
        std::any a = arr;
        /* libcis: removed (rtti: untestable on target) */  // ensure that it is decayed
    }

  return 0;

    return 0;
}
} // libcis_ns_utilities_any_any_class_any_observers_type_764b237f (libcis)

