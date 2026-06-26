// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_func_search_func_search_bmh_implicit_ctad_6f3a24c8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <functional>

// boyer_moore_horspool_searcher

// Make sure that the implicitly-generated CTAD works.

#include <functional>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_search_func_search_bmh_implicit_ctad_6f3a24c8 { // libcis
int main(int, char**) {
  {
    char const* str = "hello";
    std::boyer_moore_horspool_searcher searcher(str, str + 3);
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_horspool_searcher<char const*, std::hash<char>, std::equal_to<>>);
  }
  {
    struct myhash : std::hash<char> { };
    char const* str = "hello";
    std::boyer_moore_horspool_searcher searcher(str, str + 3, myhash{}, std::not_equal_to<>());
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_horspool_searcher<char const*, myhash, std::not_equal_to<>>);
  }
  {
    struct myhash : std::hash<char> { };
    char const* str = "hello";
    std::boyer_moore_horspool_searcher searcher(str, str + 3, myhash{});
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_horspool_searcher<char const*, myhash, std::equal_to<>>);
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_func_search_func_search_bmh_implicit_ctad_6f3a24c8 (libcis)

