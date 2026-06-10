// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_initlist_types).
// main -> test_language_support_support_initlist_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// template<class E>
// class initializer_list
// {
// public:
//     typedef E        value_type;
//     typedef const E& reference;
//     typedef const E& const_reference;
//     typedef size_t   size_type;
//
//     typedef const E* iterator;
//     typedef const E* const_iterator;

#include <initializer_list>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_language_support_support_initlist_types { // libcis: isolate file-scope helpers
struct A {};
} using namespace libcis_ns_language_support_support_initlist_types; // libcis


void test_language_support_support_initlist_types()
{
    static_assert((std::is_same<std::initializer_list<A>::value_type, A>::value), "");
    static_assert((std::is_same<std::initializer_list<A>::reference, const A&>::value), "");
    static_assert((std::is_same<std::initializer_list<A>::const_reference, const A&>::value), "");
    static_assert((std::is_same<std::initializer_list<A>::size_type, std::size_t>::value), "");
    static_assert((std::is_same<std::initializer_list<A>::iterator, const A*>::value), "");
    static_assert((std::is_same<std::initializer_list<A>::const_iterator, const A*>::value), "");

  return;
}
