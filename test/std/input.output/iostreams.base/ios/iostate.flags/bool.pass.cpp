// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_iostate_flags_bool).
// main -> test_input_output_iostreams_base_ios_iostate_flags_bool; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// operator unspecified-bool-type() const;

#include <ios>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_ios_iostate_flags_bool()
{
    std::ios ios(0);
    assert(static_cast<bool>(ios) == !ios.fail());
    ios.setstate(std::ios::failbit);
    assert(static_cast<bool>(ios) == !ios.fail());
    static_assert((!std::is_convertible<std::ios, int>::value), "");
    static_assert((!std::is_convertible<std::ios const&, int>::value), "");
#if TEST_STD_VER >= 11
    static_assert(!std::is_convertible<std::ios, void*>::value, "");
    static_assert(!std::is_convertible<std::ios, bool>::value, "");
#else
    static_assert(std::is_convertible<std::ios, void*>::value, "");
    static_assert(std::is_convertible<std::ios, bool>::value, "");
    (void)(ios == 0);  // SPEC2006 apparently relies on this to compile
#endif

  return;
}
