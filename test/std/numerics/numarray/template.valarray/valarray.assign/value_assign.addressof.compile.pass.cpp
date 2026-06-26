// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_valarray_valarray_assign_value_assign_addressof_4aa22731).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template<class T> class valarray;

// valarray& operator=(const value_type& x);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <valarray>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_numerics_numarray_template_valarray_valarray_assign_value_assign_addressof_4aa22731 { // libcis
void test() {
  std::valarray<operator_hijacker> vo;
  std::valarray<operator_hijacker> v;
  v = vo;
}
} // libcis_ns_numerics_numarray_template_valarray_valarray_assign_value_assign_addressof_4aa22731 (libcis)

