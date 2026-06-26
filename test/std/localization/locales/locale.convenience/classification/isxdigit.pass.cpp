// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isxdigit_e61a0c3e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> bool isxdigit (charT c, const locale& loc);

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_convenience_classification_isxdigit_e61a0c3e { // libcis
int main(int, char**)
{
    std::locale l;
    assert(!std::isxdigit(' ', l));
    assert(!std::isxdigit('<', l));
    assert(!std::isxdigit('\x8', l));
    assert( std::isxdigit('A', l));
    assert( std::isxdigit('a', l));
    assert(!std::isxdigit('z', l));
    assert( std::isxdigit('3', l));
    assert(!std::isxdigit('.', l));
    assert( std::isxdigit('f', l));
    assert( std::isxdigit('9', l));
    assert(!std::isxdigit('+', l));

  return 0;

    return 0;
}
} // libcis_ns_localization_locales_locale_convenience_classification_isxdigit_e61a0c3e (libcis)

