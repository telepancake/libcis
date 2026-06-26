// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_element_type_non_explicit_ctor_273466a5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// template <class U> propagate_const(U&&);

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <cassert>

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_element_type_non_explicit_ctor_273466a5 { // libcis
using std::experimental::propagate_const;

typedef propagate_const<X> P;

void f(const P&)
{
}

int main(int, char**) { f(2);
  return 0;

    return 0;
}
} // libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_element_type_non_explicit_ctor_273466a5 (libcis)

