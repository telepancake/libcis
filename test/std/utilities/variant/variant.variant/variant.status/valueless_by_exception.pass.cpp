// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_variant_variant_variant_variant_status_valueless_by_exception_509735fa).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// template <class ...Types> class variant;

// constexpr bool valueless_by_exception() const noexcept;

#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

#include "archetypes.h"
#include "test_macros.h"
#include "variant_test_helpers.h"


namespace libcis_ns_utilities_variant_variant_variant_variant_status_valueless_by_exception_509735fa { // libcis
int main(int, char**) {
  {
    using V = std::variant<int, long>;
    constexpr V v;
    static_assert(!v.valueless_by_exception(), "");
  }
  {
    using V = std::variant<int, long>;
    V v;
    assert(!v.valueless_by_exception());
  }
  {
    using V = std::variant<int, long, std::string>;
    const V v("abc");
    assert(!v.valueless_by_exception());
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    using V = std::variant<int, MakeEmptyT>;
    V v;
    assert(!v.valueless_by_exception());
    makeEmpty(v);
    assert(v.valueless_by_exception());
  }
#endif

  return 0;

    return 0;
}
} // libcis_ns_utilities_variant_variant_variant_variant_status_valueless_by_exception_509735fa (libcis)

