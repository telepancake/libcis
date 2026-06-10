// AST-transferred from libc++ by tools/transfer.py (slug=utilities_any_any_nonmembers_swap).
// main -> test_utilities_any_any_nonmembers_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <any>

// void swap(any &, any &) noexcept

// swap(...) just wraps any::swap(...). That function is tested elsewhere.

#include <any>
#include <cassert>

#include "test_macros.h"

void test_utilities_any_any_nonmembers_swap()
{

    { // test noexcept
        std::any a;
        static_assert(noexcept(swap(a, a)), "swap(any&, any&) must be noexcept");
    }
    {
        std::any a1 = 1;
        std::any a2 = 2;

        swap(a1, a2);

        assert(std::any_cast<int>(a1) == 2);
        assert(std::any_cast<int>(a2) == 1);
    }

  return;
}
