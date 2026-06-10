// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_assignment_assign).
// main -> test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_assignment_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class U> propagate_const& propagate_const::operator=(const propagate_const<U>&)=delete;

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <type_traits>

using std::experimental::propagate_const;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_assignment_assign { // libcis: isolate file-scope helpers
typedef propagate_const<X> P;
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_assignment_assign; // libcis


void test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_assignment_assign() { static_assert(!std::is_assignable<P, const P &>::value, "");
  return;
}
