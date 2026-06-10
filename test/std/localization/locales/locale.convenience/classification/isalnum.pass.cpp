// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isalnum).
// main -> test_localization_locales_locale_convenience_classification_isalnum; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool isalnum (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_isalnum()
{
    std::locale l;
    assert(!std::isalnum(' ', l));
    assert(!std::isalnum('<', l));
    assert(!std::isalnum('\x8', l));
    assert( std::isalnum('A', l));
    assert( std::isalnum('a', l));
    assert( std::isalnum('z', l));
    assert( std::isalnum('3', l));
    assert(!std::isalnum('.', l));
    assert( std::isalnum('f', l));
    assert( std::isalnum('9', l));
    assert(!std::isalnum('+', l));

  return;
}
