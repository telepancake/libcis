// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_move_sentinel_constraints_e1ae65b6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <iterator>

// template<semiregular S>
//   class move_sentinel;

#include <iterator>

namespace libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_constraints_e1ae65b6 { // libcis
template<class T>
concept HasMoveSentinel = requires {
  typename std::move_sentinel<T>;
};

struct Semiregular {};

struct NotSemiregular {
  NotSemiregular(int);
};

static_assert( HasMoveSentinel<int>);
static_assert( HasMoveSentinel<int*>);
static_assert( HasMoveSentinel<Semiregular>);
static_assert(!HasMoveSentinel<NotSemiregular>);
} // libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_constraints_e1ae65b6 (libcis)

