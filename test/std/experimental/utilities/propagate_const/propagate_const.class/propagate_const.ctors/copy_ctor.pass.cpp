// AST-transferred from libc++ by tools/transfer.py (slug=experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_copy_ctor).
// main -> test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_copy_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <propagate_const>

// propagate_const(const propagate_const&)=delete;

#include <experimental/propagate_const>
#include "test_macros.h"
#include "propagate_const_helpers.h"
#include <type_traits>

using std::experimental::propagate_const;

namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_copy_ctor { // libcis: isolate file-scope helpers
typedef propagate_const<X> P;
} using namespace libcis_ns_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_copy_ctor; // libcis


void test_experimental_utilities_propagate_const_propagate_const_class_propagate_const_ctors_copy_ctor() { static_assert(!std::is_constructible<P, const P &>::value, "");
  return;
}
