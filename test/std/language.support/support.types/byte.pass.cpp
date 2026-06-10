// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_types_byte).
// main -> test_language_support_support_types_byte; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <type_traits>
#include "test_macros.h"

// XFAIL: c++03, c++11, c++14

// std::byte is not an integer type, nor a character type.
// It is a distinct type for accessing the bits that ultimately make up object storage.

#if TEST_STD_VER > 17
namespace libcis_ns_language_support_support_types_byte { // libcis: isolate file-scope helpers
static_assert(std::is_trivially_copyable<std::byte>::value, "");
static_assert(std::is_trivially_default_constructible<std::byte>::value, "");
static_assert(std::is_standard_layout<std::byte>::value, "");
#else
static_assert( std::is_pod<std::byte>::value, "" );
#endif
static_assert(!std::is_arithmetic<std::byte>::value, "" );
static_assert(!std::is_integral<std::byte>::value, "" );

static_assert(!std::is_same<std::byte,          char>::value, "" );
static_assert(!std::is_same<std::byte,   signed char>::value, "" );
static_assert(!std::is_same<std::byte, unsigned char>::value, "" );

// The standard doesn't outright say this, but it's pretty clear that it has to be true.
static_assert(sizeof(std::byte) == 1, "" );
} using namespace libcis_ns_language_support_support_types_byte; // libcis


void test_language_support_support_types_byte() {
  return;
}
