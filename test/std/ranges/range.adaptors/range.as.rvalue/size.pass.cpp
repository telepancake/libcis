// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_as_rvalue_size).
// main -> test_ranges_range_adaptors_range_as_rvalue_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// constexpr auto size()
// constexpr auto size() const

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <cstddef>
#include <ranges>

namespace libcis_ns_ranges_range_adaptors_range_as_rvalue_size { // libcis: isolate file-scope helpers
struct ConstSizedView : std::ranges::view_base {
  bool* size_called;
  int* begin() const;
  int* end() const;

  constexpr std::size_t size() const {
    *size_called = true;
    return 3;
  }
};

struct SizedView : std::ranges::view_base {
  bool* size_called;
  int* begin() const;
  int* end() const;

  constexpr int size() {
    *size_called = true;
    return 5;
  }
};

struct UnsizedView : std::ranges::view_base {
  int* begin() const;
  int* end() const;
};

template <class T>
concept HasSize = requires(T v) { v.size(); };

static_assert(HasSize<ConstSizedView>);
static_assert(HasSize<const ConstSizedView>);
static_assert(HasSize<SizedView>);
static_assert(!HasSize<const SizedView>);
static_assert(!HasSize<UnsizedView>);
static_assert(!HasSize<const UnsizedView>);

constexpr bool test() {
  {
    bool size_called = false;
    std::ranges::as_rvalue_view view(ConstSizedView{{}, &size_called});
    std::same_as<std::size_t> auto size = view.size();
    assert(size == 3);
    assert(size_called);
  }

  {
    bool size_called = false;
    std::ranges::as_rvalue_view view(SizedView{{}, &size_called});
    std::same_as<int> auto size = view.size();
    assert(size == 5);
    assert(size_called);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_as_rvalue_size; // libcis


void test_ranges_range_adaptors_range_as_rvalue_size() {
  test();
  static_assert(test());

  return;
}
