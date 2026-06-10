// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isprint).
// main -> test_localization_locales_locale_convenience_classification_isprint; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool isprint (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_isprint()
{
    std::locale l;
    assert( std::isprint(' ', l));
    assert( std::isprint('<', l));
    assert(!std::isprint('\x8', l));
    assert( std::isprint('A', l));
    assert( std::isprint('a', l));
    assert( std::isprint('z', l));
    assert( std::isprint('3', l));
    assert( std::isprint('.', l));
    assert( std::isprint('f', l));
    assert( std::isprint('9', l));
    assert( std::isprint('+', l));

  return;
}
