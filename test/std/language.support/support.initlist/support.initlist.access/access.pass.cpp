// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_initlist_support_initlist_access_access).
// main -> test_language_support_support_initlist_support_initlist_access_access; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// template<class E> class initializer_list;

// const E* begin() const;
// const E* end() const;
// size_t size() const;

#include <initializer_list>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_language_support_support_initlist_support_initlist_access_access { // libcis: isolate file-scope helpers
struct A
{
    A(std::initializer_list<int> il)
    {
        const int* b = il.begin();
        const int* e = il.end();
        assert(il.size() == 3);
        assert(static_cast<std::size_t>(e - b) == il.size());
        assert(*b++ == 3);
        assert(*b++ == 2);
        assert(*b++ == 1);
    }
};

#if TEST_STD_VER > 11
struct B
{
    constexpr B(std::initializer_list<int> il)
    {
        const int* b = il.begin();
        const int* e = il.end();
        assert(il.size() == 3);
        assert(static_cast<std::size_t>(e - b) == il.size());
        assert(*b++ == 3);
        assert(*b++ == 2);
        assert(*b++ == 1);
    }
};
} using namespace libcis_ns_language_support_support_initlist_support_initlist_access_access; // libcis


#endif // TEST_STD_VER > 11

void test_language_support_support_initlist_support_initlist_access_access()
{
    A test1 = {3, 2, 1}; (void)test1;
#if TEST_STD_VER > 11
    constexpr B test2 = {3, 2, 1};
    (void)test2;
#endif // TEST_STD_VER > 11

  return;
}
