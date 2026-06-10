// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_islower).
// main -> test_localization_locales_locale_convenience_classification_islower; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool islower (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_islower()
{
    std::locale l;
    assert(!std::islower(' ', l));
    assert(!std::islower('<', l));
    assert(!std::islower('\x8', l));
    assert(!std::islower('A', l));
    assert( std::islower('a', l));
    assert( std::islower('z', l));
    assert(!std::islower('3', l));
    assert(!std::islower('.', l));
    assert( std::islower('f', l));
    assert(!std::islower('9', l));
    assert(!std::islower('+', l));

  return;
}
