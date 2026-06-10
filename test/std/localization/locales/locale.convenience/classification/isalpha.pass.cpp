// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isalpha).
// main -> test_localization_locales_locale_convenience_classification_isalpha; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool isalpha (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_isalpha()
{
    std::locale l;
    assert(!std::isalpha(' ', l));
    assert(!std::isalpha('<', l));
    assert(!std::isalpha('\x8', l));
    assert( std::isalpha('A', l));
    assert( std::isalpha('a', l));
    assert( std::isalpha('z', l));
    assert(!std::isalpha('3', l));
    assert(!std::isalpha('.', l));
    assert( std::isalpha('f', l));
    assert(!std::isalpha('9', l));
    assert(!std::isalpha('+', l));

  return;
}
