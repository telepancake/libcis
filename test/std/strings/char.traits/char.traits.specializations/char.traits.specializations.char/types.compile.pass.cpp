// AST-transferred from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char_types).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char>

// using char_type           = char;
// using int_type            = int;
// using off_type            = streamoff;
// using pos_type            = streampos;
// using state_type          = mbstate_t;
// using comparison_category = strong_ordering;

#include <string>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types { // libcis: isolate file-scope helpers
static_assert(std::is_same<std::char_traits<char>::char_type, char>::value, "");
static_assert(std::is_same<std::char_traits<char>::int_type, int>::value, "");
static_assert(std::is_same<std::char_traits<char>::off_type, std::streamoff>::value, "");
static_assert(std::is_same<std::char_traits<char>::pos_type, std::streampos>::value, "");
static_assert(std::is_same<std::char_traits<char>::state_type, std::mbstate_t>::value, "");
#if TEST_STD_VER > 17
static_assert(std::is_same_v<std::char_traits<char>::comparison_category, std::strong_ordering>);
} using namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types; // libcis

#endif
