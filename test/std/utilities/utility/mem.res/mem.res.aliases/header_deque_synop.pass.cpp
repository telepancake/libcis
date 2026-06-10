// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_aliases_header_deque_synop).
// main -> test_utilities_utility_mem_res_mem_res_aliases_header_deque_synop; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://llvm.org/PR40995 is fixed
// UNSUPPORTED: availability-pmr-missing

// <deque>

// namespace std::pmr {
// template <class T>
// using deque =
//     ::std::deque<T, polymorphic_allocator<T>>
//
// } // namespace std::pmr

#include <deque>
#include <memory_resource>
#include <type_traits>
#include <cassert>

void test_utilities_utility_mem_res_mem_res_aliases_header_deque_synop() {
  using StdDeque = std::deque<int, std::pmr::polymorphic_allocator<int>>;
  using PmrDeque = std::pmr::deque<int>;
  static_assert(std::is_same<StdDeque, PmrDeque>::value, "");
  PmrDeque d;
  assert(d.get_allocator().resource() == std::pmr::get_default_resource());

  return;
}
