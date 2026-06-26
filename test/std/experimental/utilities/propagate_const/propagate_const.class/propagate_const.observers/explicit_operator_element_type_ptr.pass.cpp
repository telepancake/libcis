// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_observers_explicit_operator_element_type_ptr_b3ee8987).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// propagate_const::operator const element_type*() const;

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <type_traits>

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_observers_explicit_operator_element_type_ptr_b3ee8987 { // libcis
using std::experimental::propagate_const;

typedef propagate_const<X> P;

int main(int, char**) {
  static_assert(!std::is_convertible<const P, const int *>::value, "");

  return 0;

    return 0;
}
} // libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_observers_explicit_operator_element_type_ptr_b3ee8987 (libcis)

