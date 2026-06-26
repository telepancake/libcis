// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_rtti_bad_typeid_bad_typeid_1dc38c4e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test bad_typeid

#include <typeinfo>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_rtti_bad_typeid_bad_typeid_1dc38c4e { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::exception, std::bad_typeid>::value),
                 "std::is_base_of<std::exception, std::bad_typeid>::value");
    static_assert(std::is_polymorphic<std::bad_typeid>::value,
                 "std::is_polymorphic<std::bad_typeid>::value");
    std::bad_typeid b;
    std::bad_typeid b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_rtti_bad_typeid_bad_typeid_1dc38c4e (libcis)

