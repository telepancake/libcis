// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isspace).
// main -> test_localization_locales_locale_convenience_classification_isspace; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool isspace (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_isspace()
{
    std::locale l;
    assert( std::isspace(' ', l));
    assert(!std::isspace('<', l));
    assert(!std::isspace('\x8', l));
    assert(!std::isspace('A', l));
    assert(!std::isspace('a', l));
    assert(!std::isspace('z', l));
    assert(!std::isspace('3', l));
    assert(!std::isspace('.', l));
    assert(!std::isspace('f', l));
    assert(!std::isspace('9', l));
    assert(!std::isspace('+', l));

  return;
}
