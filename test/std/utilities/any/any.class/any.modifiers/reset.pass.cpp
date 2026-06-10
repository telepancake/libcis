// AST-transferred from libc++ by tools/transfer.py (slug=utilities_any_any_class_any_modifiers_reset).
// main -> test_utilities_any_any_class_any_modifiers_reset; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <any>

// any::reset() noexcept

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"

void test_utilities_any_any_class_any_modifiers_reset()
{
    // empty
    {
        std::any a;
        ASSERT_NOEXCEPT(a.reset());

        assertEmpty(a);

        a.reset();

        assertEmpty(a);
    }
    // small object
    {
        std::any a = small(1);
        assert(small::count == 1);
        assertContains<small>(a, 1);

        a.reset();

        assertEmpty<small>(a);
        assert(small::count == 0);
    }
    // large object
    {
        std::any a = large(1);
        assert(large::count == 1);
        assertContains<large>(a, 1);

        a.reset();

        assertEmpty<large>(a);
        assert(large::count == 0);
    }

  return;
}
