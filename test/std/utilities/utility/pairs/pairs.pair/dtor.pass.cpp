// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pairs_pair_dtor_c47eb2ca).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <utility>

// template <class T1, class T2> struct pair

// ~pair()

// C++17 added:
//   The destructor of pair shall be a trivial destructor
//     if (is_trivially_destructible_v<T1> && is_trivially_destructible_v<T2>) is true.


#include <utility>
#include <type_traits>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_utility_pairs_pairs_pair_dtor_c47eb2ca { // libcis
int main(int, char**)
{
  static_assert((std::is_trivially_destructible<
      std::pair<int, float> >::value), "");
  static_assert((!std::is_trivially_destructible<
      std::pair<int, std::string> >::value), "");

  return 0;

    return 0;
}
} // libcis_ns_utilities_utility_pairs_pairs_pair_dtor_c47eb2ca (libcis)

