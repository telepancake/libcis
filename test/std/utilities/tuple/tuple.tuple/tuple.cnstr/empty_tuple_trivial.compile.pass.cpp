// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_cnstr_empty_tuple_trivial_e3ec4160).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

#include <tuple>
#include <type_traits>

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_cnstr_empty_tuple_trivial_e3ec4160 { // libcis
static_assert(std::is_trivially_constructible<std::tuple<>>::value, "");
} // libcis_ns_utilities_tuple_tuple_tuple_tuple_cnstr_empty_tuple_trivial_e3ec4160 (libcis)

