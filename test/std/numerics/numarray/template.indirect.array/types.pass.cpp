// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_template_indirect_array_types_de29ea93).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <valarray>

// template <class T>
// class indirect_array
// {
// public:
//     typedef T value_type;

#include <valarray>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_numerics_numarray_template_indirect_array_types_de29ea93 { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::indirect_array<int>::value_type, int>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_template_indirect_array_types_de29ea93 (libcis)

