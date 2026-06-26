// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_dereference_b6fb12fb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// element_type& propagate_const::operator*();

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <cassert>

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_dereference_b6fb12fb { // libcis
using std::experimental::propagate_const;

typedef propagate_const<X> P;

constexpr P f()
{
  P p(1);
  *p = 2;
  return p;
}

int main(int, char**) {
  constexpr P p = f();
  static_assert(*p==2,"");

  return 0;

    return 0;
}
} // libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_non_const_observers_dereference_b6fb12fb (libcis)

