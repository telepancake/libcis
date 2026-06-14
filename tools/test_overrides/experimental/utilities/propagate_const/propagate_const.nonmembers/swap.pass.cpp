// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_nonmembers_swap).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class T> constexpr void swap(propagate_const<T>& x, propagate_const<T>& y);

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <cassert>

bool swap_called = false;  // libcis: hoisted with swap (below)

// libcis: hoisted to global scope -- the transfer wrapped this ADL
// customization in libcis_ns_* but X is global, so member swap's
// `using std::swap; swap(x_, ...)` could not find it via ADL.
void swap(X &, X &) { swap_called = true; }


namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_nonmembers_swap { // libcis
using std::experimental::propagate_const;

int main(int, char**) {
  typedef propagate_const<X> P;
  P p1(1);
  P p2(2);
  swap(p1, p2);
  assert(swap_called);

  return 0;
}
} // libcis_ns_experimental_utilities_propagate_const_propagate_const_nonmembers_swap (libcis)

