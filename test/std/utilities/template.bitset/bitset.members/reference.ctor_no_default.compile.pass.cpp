// AST-transferred from libc++ by tools/transfer.py (slug=utilities_template_bitset_bitset_members_reference_ctor_no_default).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <bitset>

#include <bitset>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_template_bitset_bitset_members_reference_ctor_no_default { // libcis: isolate file-scope helpers
static_assert(!std::is_default_constructible<std::bitset<0>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<1>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<8>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<12>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<16>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<24>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<32>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<48>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<64>::reference>::value, "");
static_assert(!std::is_default_constructible<std::bitset<96>::reference>::value, "");

#if TEST_STD_VER >= 11
void test_no_ambiguity_among_default_constructors(std::enable_if<false>);
void test_no_ambiguity_among_default_constructors(std::bitset<0>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<1>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<8>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<12>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<16>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<24>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<32>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<48>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<64>::reference);
void test_no_ambiguity_among_default_constructors(std::bitset<96>::reference);

ASSERT_SAME_TYPE(decltype(test_no_ambiguity_among_default_constructors({})), void);
} using namespace libcis_ns_utilities_template_bitset_bitset_members_reference_ctor_no_default; // libcis

#endif
