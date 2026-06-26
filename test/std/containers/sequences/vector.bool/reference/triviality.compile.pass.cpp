// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_reference_triviality_36957bb8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <type_traits>
#include <vector>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_bool_reference_triviality_36957bb8 { // libcis
using Ref = std::vector<bool>::reference;

LIBCPP_STATIC_ASSERT(!std::is_trivially_constructible<Ref>::value, "");
LIBCPP_STATIC_ASSERT(std::is_trivially_copy_constructible<Ref>::value, "");
LIBCPP_STATIC_ASSERT(std::is_trivially_move_constructible<Ref>::value, "");
LIBCPP_STATIC_ASSERT(!std::is_trivially_copy_assignable<Ref>::value, "");
LIBCPP_STATIC_ASSERT(!std::is_trivially_move_assignable<Ref>::value, "");
LIBCPP_STATIC_ASSERT(std::is_trivially_destructible<Ref>::value, "");
} // libcis_ns_containers_sequences_vector_bool_reference_triviality_36957bb8 (libcis)

