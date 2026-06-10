// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_unord_hash_pointer).
// main -> test_utilities_function_objects_unord_hash_pointer; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <functional>

// template <class T>
// struct hash
//     : public unary_function<T, size_t>
// {
//     size_t operator()(T val) const;
// };

// Not very portable

#include <cassert>
#include <cstddef>
#include <functional>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_unord_hash_pointer { // libcis: isolate file-scope helpers
template <class T>
void
test()
{
    typedef std::hash<T> H;
#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename H::argument_type, T>::value), "");
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "");
#endif
    ASSERT_NOEXCEPT(H()(T()));
    H h;

    typedef typename std::remove_pointer<T>::type type;
    type i;
    type j;
    assert(h(&i) != h(&j));
}

void test_nullptr() {
  typedef std::nullptr_t T;
  typedef std::hash<T> H;
#if TEST_STD_VER <= 17
  static_assert((std::is_same<typename H::argument_type, T>::value), "");
  static_assert((std::is_same<typename H::result_type, std::size_t>::value), "");
#endif
  ASSERT_NOEXCEPT(H()(T()));
}
} using namespace libcis_ns_utilities_function_objects_unord_hash_pointer; // libcis


void test_utilities_function_objects_unord_hash_pointer()
{
    test<int*>();
    test_nullptr();

  return;
}
