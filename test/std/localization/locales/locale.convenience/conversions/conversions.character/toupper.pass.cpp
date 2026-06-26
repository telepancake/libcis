// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_conversions_conversions_character_toupper_d71a3230).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> charT toupper(charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_convenience_conversions_conversions_character_toupper_d71a3230 { // libcis
int main(int, char**)
{
    std::locale l;
    assert(std::toupper(' ', l) == ' ');
    assert(std::toupper('<', l) == '<');
    assert(std::toupper('\x8', l) == '\x8');
    assert(std::toupper('A', l) == 'A');
    assert(std::toupper('a', l) == 'A');
    assert(std::toupper('z', l) == 'Z');
    assert(std::toupper('3', l) == '3');
    assert(std::toupper('.', l) == '.');
    assert(std::toupper('f', l) == 'F');
    assert(std::toupper('9', l) == '9');
    assert(std::toupper('+', l) == '+');

  return 0;

    return 0;
}
} // libcis_ns_localization_locales_locale_convenience_conversions_conversions_character_toupper_d71a3230 (libcis)

