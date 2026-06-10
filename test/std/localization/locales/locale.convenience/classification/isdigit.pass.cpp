// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isdigit).
// main -> test_localization_locales_locale_convenience_classification_isdigit; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool isdigit (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_isdigit()
{
    std::locale l;
    assert(!std::isdigit(' ', l));
    assert(!std::isdigit('<', l));
    assert(!std::isdigit('\x8', l));
    assert(!std::isdigit('A', l));
    assert(!std::isdigit('a', l));
    assert(!std::isdigit('z', l));
    assert( std::isdigit('3', l));
    assert(!std::isdigit('.', l));
    assert(!std::isdigit('f', l));
    assert( std::isdigit('9', l));
    assert(!std::isdigit('+', l));

  return;
}
