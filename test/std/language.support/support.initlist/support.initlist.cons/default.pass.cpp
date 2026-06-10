// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_initlist_support_initlist_cons_default).
// main -> test_language_support_support_initlist_support_initlist_cons_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// template<class E> class initializer_list;

// initializer_list();

#include <initializer_list>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_initlist_support_initlist_cons_default { // libcis: isolate file-scope helpers
struct A {};
} using namespace libcis_ns_language_support_support_initlist_support_initlist_cons_default; // libcis


void test_language_support_support_initlist_support_initlist_cons_default()
{
    std::initializer_list<A> il;
    assert(il.size() == 0);

#if TEST_STD_VER > 11
    constexpr std::initializer_list<A> il2;
    static_assert(il2.size() == 0, "");
#endif // TEST_STD_VER > 11

  return;
}
