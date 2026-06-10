// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_get).
// main -> test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_get; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// element_type* propagate_const::get();

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <cassert>

using std::experimental::propagate_const;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_get { // libcis: isolate file-scope helpers
typedef propagate_const<X> P;

constexpr P f()
{
  P p(1);
  *p.get() = 2;
  return p;
}
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_get; // libcis


void test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_get() {
  constexpr P p = f();
  static_assert(*(p.get())==2,"");

  return;
}
