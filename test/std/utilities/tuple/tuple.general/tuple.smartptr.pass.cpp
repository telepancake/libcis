// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_general_tuple_smartptr).
// main -> test_utilities_tuple_tuple_general_tuple_smartptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//

// UNSUPPORTED: c++03

//  Tuples of smart pointers; based on bug #18350
//  auto_ptr doesn't have a copy constructor that takes a const &, but tuple does.

#include <tuple>
#include <memory>

#include "test_macros.h"

void test_utilities_tuple_tuple_general_tuple_smartptr() {
    {
    std::tuple<std::unique_ptr<char>> up;
    std::tuple<std::shared_ptr<char>> sp;
    std::tuple<std::weak_ptr  <char>> wp;
    }
    {
    std::tuple<std::unique_ptr<char[]>> up;
    std::tuple<std::shared_ptr<char[]>> sp;
    std::tuple<std::weak_ptr  <char[]>> wp;
    }
    // Smart pointers of type 'T[N]' are not tested here since they are not
    // supported by the standard nor by libc++'s implementation.
    // See https://reviews.llvm.org/D21320 for more information.

  return;
}
