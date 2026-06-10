// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_bind_func_bind_func_bind_isplace_specialization).
// main -> test_utilities_function_objects_bind_func_bind_func_bind_isplace_specialization; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <functional>

// template<class T> struct is_placeholder;
//   A program may specialize this template for a program-defined type T
//   to have a base characteristic of integral_constant<int, N> with N > 0
//   to indicate that T should be treated as a placeholder type.
//   https://llvm.org/PR51753

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

namespace libcis_ns_utilities_function_objects_bind_func_bind_func_bind_isplace_specialization { // libcis: isolate file-scope helpers
struct My2 {};
} using namespace libcis_ns_utilities_function_objects_bind_func_bind_func_bind_isplace_specialization; // libcis

template<> struct std::is_placeholder<My2> : std::integral_constant<int, 2> {};

void test_utilities_function_objects_bind_func_bind_func_bind_isplace_specialization()
{
  {
    auto f = [](auto x) { return 10*x + 9; };
    My2 place;
    auto bound = std::bind(f, place);
    assert(bound(7, 8) == 89);
  }
  {
    auto f = [](auto x) { return 10*x + 9; };
    const My2 place;
    auto bound = std::bind(f, place);
    assert(bound(7, 8) == 89);
  }
  {
    auto f = [](auto x) { return 10*x + 9; };
    My2 place;
    auto bound = std::bind(f, std::move(place));
    assert(bound(7, 8) == 89);
  }
  {
    auto f = [](auto x) { return 10*x + 9; };
    const My2 place;
    auto bound = std::bind(f, std::move(place));
    assert(bound(7, 8) == 89);
  }

  return;
}
