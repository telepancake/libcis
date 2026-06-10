// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_ctor_copy_move).
// main -> test_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_ctor_copy_move; file-scope helpers isolated in anon namespace.
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

// <memory_resource>

// monotonic_buffer_resource(monotonic_buffer_resource const&) = delete;
// monotonic_buffer_resource& operator=(monotonic_buffer_resource const&) = delete;

#include <memory_resource>
#include <type_traits>

void test_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_ctor_copy_move() {
  using MBR = std::pmr::monotonic_buffer_resource;
  static_assert(!std::is_copy_constructible_v<MBR>);
  static_assert(!std::is_move_constructible_v<MBR>);
  static_assert(!std::is_copy_assignable_v<MBR>);
  static_assert(!std::is_move_assignable_v<MBR>);

  return;
}
