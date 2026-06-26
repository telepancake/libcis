// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_comparisons_constexpr_init_3be9bd70).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <functional>

// equal_to, not_equal_to, less, et al.

// Test that these types can be constructed w/o an initializer in a constexpr
// context. This is specifically testing gcc.gnu.org/PR83921


#include <functional>
#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_comparisons_constexpr_init_3be9bd70 { // libcis
template <class T>
constexpr bool test_constexpr_context() {
  std::equal_to<T> eq;
  ((void)eq);
  std::not_equal_to<T> neq;
  ((void)neq);
  std::less<T> l;
  ((void)l);
  std::less_equal<T> le;
  ((void)le);
  std::greater<T> g;
  ((void)g);
  std::greater_equal<T> ge;
  ((void)ge);
  return true;
}

static_assert(test_constexpr_context<int>(), "");
static_assert(test_constexpr_context<void>(), "");


int main(int, char**) {


  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_comparisons_constexpr_init_3be9bd70 (libcis)

