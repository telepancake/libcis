// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_iscntrl).
// main -> test_localization_locales_locale_convenience_classification_iscntrl; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool iscntrl (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locales_locale_convenience_classification_iscntrl()
{
    std::locale l;
    assert(!std::iscntrl(' ', l));
    assert(!std::iscntrl('<', l));
    assert( std::iscntrl('\x8', l));
    assert(!std::iscntrl('A', l));
    assert(!std::iscntrl('a', l));
    assert(!std::iscntrl('z', l));
    assert(!std::iscntrl('3', l));
    assert(!std::iscntrl('.', l));
    assert(!std::iscntrl('f', l));
    assert(!std::iscntrl('9', l));
    assert(!std::iscntrl('+', l));

  return;
}
