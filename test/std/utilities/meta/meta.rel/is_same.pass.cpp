// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_rel_is_same).
// main -> test_utilities_meta_meta_rel_is_same; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_same

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_rel_is_same { // libcis: isolate file-scope helpers
template <class T, class U>
void test_is_same()
{
    static_assert(( std::is_same<T, U>::value), "");
    static_assert((!std::is_same<const T, U>::value), "");
    static_assert((!std::is_same<T, const U>::value), "");
    static_assert(( std::is_same<const T, const U>::value), "");
#if TEST_STD_VER > 14
    static_assert(( std::is_same_v<T, U>), "");
    static_assert((!std::is_same_v<const T, U>), "");
    static_assert((!std::is_same_v<T, const U>), "");
    static_assert(( std::is_same_v<const T, const U>), "");
#endif
}

template <class T, class U>
void test_is_same_ref()
{
    static_assert((std::is_same<T, U>::value), "");
    static_assert((std::is_same<const T, U>::value), "");
    static_assert((std::is_same<T, const U>::value), "");
    static_assert((std::is_same<const T, const U>::value), "");
#if TEST_STD_VER > 14
    static_assert((std::is_same_v<T, U>), "");
    static_assert((std::is_same_v<const T, U>), "");
    static_assert((std::is_same_v<T, const U>), "");
    static_assert((std::is_same_v<const T, const U>), "");
#endif
}

template <class T, class U>
void test_is_not_same()
{
    static_assert((!std::is_same<T, U>::value), "");
}

template <class T>
struct OverloadTest
{
    void fn(std::is_same<T, int>) { }
    void fn(std::false_type) { }
    void x() { fn(std::false_type()); }
};

class Class
{
public:
    ~Class();
};
} using namespace libcis_ns_utilities_meta_meta_rel_is_same; // libcis


void test_utilities_meta_meta_rel_is_same()
{
    test_is_same<int, int>();
    test_is_same<void, void>();
    test_is_same<Class, Class>();
    test_is_same<int*, int*>();
    test_is_same_ref<int&, int&>();

    test_is_not_same<int, void>();
    test_is_not_same<void, Class>();
    test_is_not_same<Class, int*>();
    test_is_not_same<int*, int&>();
    test_is_not_same<int&, int>();

    OverloadTest<char> t;
    (void)t;

  return;
}
