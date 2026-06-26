// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_special_swap_addressof_02cedaf1).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// template <class T, class Alloc>
//   void swap(list<T,Alloc>& x, list<T,Alloc>& y);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <list>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_sequences_list_list_special_swap_addressof_02cedaf1 { // libcis
void test() {
  std::list<operator_hijacker> lo;
  std::list<operator_hijacker> l;
  swap(l, lo);
}
} // libcis_ns_containers_sequences_list_list_special_swap_addressof_02cedaf1 (libcis)

