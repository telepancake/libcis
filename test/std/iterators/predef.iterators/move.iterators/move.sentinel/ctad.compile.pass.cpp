// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_move_sentinel_ctad_91d0ffc2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <iterator>
#include <type_traits>

#include <test_iterators.h>

namespace libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_ctad_91d0ffc2 { // libcis
static_assert(std::is_same_v<decltype(std::move_sentinel(std::default_sentinel_t{})),
                             std::move_sentinel<std::default_sentinel_t>>);

static_assert(
    std::is_same_v<decltype(std::move_sentinel(sentinel_wrapper<int*>{})), std::move_sentinel<sentinel_wrapper<int*>>>);
} // libcis_ns_iterators_predef_iterators_move_iterators_move_sentinel_ctad_91d0ffc2 (libcis)

