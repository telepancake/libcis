// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_addressof_2d3f2ea7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03 && !stdlib=libc++

// <vector>

// Validate various member functions of std::vector with an ADL-hijacking operator&

#include <vector>
#include <utility>

#include "operator_hijacker.h"
#include "test_iterators.h"

namespace libcis_ns_containers_sequences_vector_addressof_2d3f2ea7 { // libcis
using Vector = std::vector<operator_hijacker>;

void test(
    Vector v, Vector::const_iterator it, cpp17_input_iterator<operator_hijacker*> other_it, operator_hijacker val) {
  // emplace / insert
  v.emplace(it);
  v.insert(it, it, it);
  v.insert(it, other_it, other_it);
  v.insert(it, operator_hijacker());
  v.insert(it, 1, val);
  v.insert(it, val);

  // erase
  v.erase(it);
  v.erase(it, it);

  // assignment
  v = static_cast<Vector&>(v);
  v = std::move(v);

  // construction
  { Vector v2(std::move(v)); }
  { Vector v2(std::move(v), std::allocator<operator_hijacker>()); }

  // swap
  v.swap(v);
}
} // libcis_ns_containers_sequences_vector_addressof_2d3f2ea7 (libcis)

