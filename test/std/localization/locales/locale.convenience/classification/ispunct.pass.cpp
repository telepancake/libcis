// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_ispunct).
// main -> test_localization_locales_locale_convenience_classification_ispunct; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool ispunct (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_ispunct()
{
    std::locale l;
    assert(!std::ispunct(' ', l));
    assert( std::ispunct('<', l));
    assert(!std::ispunct('\x8', l));
    assert(!std::ispunct('A', l));
    assert(!std::ispunct('a', l));
    assert(!std::ispunct('z', l));
    assert(!std::ispunct('3', l));
    assert( std::ispunct('.', l));
    assert(!std::ispunct('f', l));
    assert(!std::ispunct('9', l));
    assert( std::ispunct('+', l));

  return;
}
