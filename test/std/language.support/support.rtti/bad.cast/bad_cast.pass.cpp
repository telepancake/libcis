// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_rtti_bad_cast_bad_cast).
// main -> test_language_support_support_rtti_bad_cast_bad_cast; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test bad_cast

#include <typeinfo>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

void test_language_support_support_rtti_bad_cast_bad_cast()
{
    static_assert((std::is_base_of<std::exception, std::bad_cast>::value),
                 "std::is_base_of<std::exception, std::bad_cast>::value");
    static_assert(std::is_polymorphic<std::bad_cast>::value,
                 "std::is_polymorphic<std::bad_cast>::value");
    std::bad_cast b;
    std::bad_cast b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);

  return;
}
