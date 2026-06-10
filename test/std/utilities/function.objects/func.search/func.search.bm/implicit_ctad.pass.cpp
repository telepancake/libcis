// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_search_func_search_bm_implicit_ctad).
// main -> test_utilities_function_objects_func_search_func_search_bm_implicit_ctad; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <functional>

// boyer_moore_searcher

// Make sure that the implicitly-generated CTAD works.

#include <functional>

#include "test_macros.h"

void test_utilities_function_objects_func_search_func_search_bm_implicit_ctad() {
  {
    char const* str = "hello";
    std::boyer_moore_searcher searcher(str, str + 3);
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_searcher<char const*, std::hash<char>, std::equal_to<>>);
  }
  {
    struct myhash : std::hash<char> { };
    char const* str = "hello";
    std::boyer_moore_searcher searcher(str, str + 3, myhash{}, std::not_equal_to<>());
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_searcher<char const*, myhash, std::not_equal_to<>>);
  }
  {
    struct myhash : std::hash<char> { };
    char const* str = "hello";
    std::boyer_moore_searcher searcher(str, str + 3, myhash{});
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_searcher<char const*, myhash, std::equal_to<>>);
  }

  return;
}
