// AST-transferred from libc++ by tools/transfer.py (slug=utilities_optional_optional_object_optional_object_ctor_nullopt_t).
// main -> test_utilities_optional_optional_object_optional_object_ctor_nullopt_t; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr optional(nullopt_t) noexcept;

#include <cassert>
#include <optional>
#include <type_traits>

#include "archetypes.h"

#include "test_macros.h"

using std::nullopt;
using std::nullopt_t;
using std::optional;

namespace libcis_ns_utilities_optional_optional_object_optional_object_ctor_nullopt_t { // libcis: isolate file-scope helpers
template <class Opt>
void test_constexpr() {
  static_assert(std::is_nothrow_constructible<Opt, nullopt_t&>::value, "");
  static_assert(std::is_trivially_destructible<Opt>::value, "");
  static_assert(std::is_trivially_destructible<typename Opt::value_type>::value, "");

  constexpr Opt opt(nullopt);
  static_assert(static_cast<bool>(opt) == false, "");

  struct test_constexpr_ctor : public Opt {
    constexpr test_constexpr_ctor() {}
  };
}

template <class Opt>
void test() {
  static_assert(std::is_nothrow_constructible<Opt, nullopt_t&>::value, "");
  static_assert(!std::is_trivially_destructible<Opt>::value, "");
  static_assert(!std::is_trivially_destructible<typename Opt::value_type>::value, "");
  {
    Opt opt(nullopt);
    assert(static_cast<bool>(opt) == false);
  }
  {
    const Opt opt(nullopt);
    assert(static_cast<bool>(opt) == false);
  }
  struct test_constexpr_ctor : public Opt {
    constexpr test_constexpr_ctor() {}
  };
}
} using namespace libcis_ns_utilities_optional_optional_object_optional_object_ctor_nullopt_t; // libcis


void test_utilities_optional_optional_object_optional_object_ctor_nullopt_t() {
  test_constexpr<optional<int>>();
  test_constexpr<optional<int*>>();
  test_constexpr<optional<ImplicitTypes::NoCtors>>();
  test_constexpr<optional<NonTrivialTypes::NoCtors>>();
  test_constexpr<optional<NonConstexprTypes::NoCtors>>();
  test<optional<NonLiteralTypes::NoCtors>>();

  return;
}
