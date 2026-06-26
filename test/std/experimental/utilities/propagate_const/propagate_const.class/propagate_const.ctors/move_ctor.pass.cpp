// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_move_ctor_6d48ec1a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// propagate_const(propagate_const&&)=default;

#include <experimental/propagate_const>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "propagate_const_helpers.h"

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_move_ctor_6d48ec1a { // libcis
using std::experimental::propagate_const;

int main(int, char**) {

  typedef propagate_const<X> P;

  P p1(2);
  P p2(std::move(p1));
  assert(*p2 == 2);

  return 0;

    return 0;
}
} // libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_move_ctor_6d48ec1a (libcis)

