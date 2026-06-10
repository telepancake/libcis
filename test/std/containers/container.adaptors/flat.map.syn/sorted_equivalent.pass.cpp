// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_flat_map_syn_sorted_equivalent).
// main -> test_containers_container_adaptors_flat_map_syn_sorted_equivalent; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// struct sorted_equivalent_t { explicit sorted_equivalent_t() = default; };
// inline constexpr sorted_equivalent_t sorted_equivalent{};

#include <cassert>
#include <concepts>
#include <flat_map>
#include <type_traits>

namespace libcis_ns_containers_container_adaptors_flat_map_syn_sorted_equivalent { // libcis: isolate file-scope helpers
template <class T>
void implicit_test(T) {}

template <class T>
concept HasImplicitDefaultCtor = requires { implicit_test<T>({}); };

static_assert(std::is_default_constructible_v<std::sorted_equivalent_t>);
static_assert(std::is_trivially_default_constructible_v<std::sorted_equivalent_t>);
static_assert(!HasImplicitDefaultCtor<std::sorted_equivalent_t>);

constexpr bool test() {
  {
    [[maybe_unused]] std::sorted_equivalent_t s;
  }
  {
    [[maybe_unused]] std::same_as<const std::sorted_equivalent_t&> decltype(auto) s = (std::sorted_equivalent);
  }
  {
    [[maybe_unused]] std::same_as<const std::sorted_equivalent_t> decltype(auto) copy = std::sorted_equivalent;
  }

  return true;
}
} using namespace libcis_ns_containers_container_adaptors_flat_map_syn_sorted_equivalent; // libcis


void test_containers_container_adaptors_flat_map_syn_sorted_equivalent() {
  test();
  static_assert(test());

  return;
}
