// AST-transferred from libc++ by tools/transfer.py (slug=utilities_variant_variant_bad_variant_access_bad_variant_access).
// main -> test_utilities_variant_variant_bad_variant_access_bad_variant_access; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <variant>

/*

 class bad_variant_access : public exception {
public:
  bad_variant_access() noexcept;
  virtual const char* what() const noexcept;
};

*/

#include <cassert>
#include <exception>
#include <type_traits>
#include <variant>

#include "test_macros.h"

void test_utilities_variant_variant_bad_variant_access_bad_variant_access() {
  static_assert(std::is_base_of<std::exception, std::bad_variant_access>::value,
                "");
  static_assert(noexcept(std::bad_variant_access{}), "must be noexcept");
  static_assert(noexcept(std::bad_variant_access{}.what()), "must be noexcept");
  std::bad_variant_access ex;
  assert(ex.what());

  return;
}
