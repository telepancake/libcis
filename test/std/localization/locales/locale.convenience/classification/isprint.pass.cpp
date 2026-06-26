// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_classification_isprint_7deac00c).
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

namespace libcis_ns_localization_locales_locale_convenience_classification_isprint_7deac00c { // libcis
int main(int, char**)
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

  return 0;

    return 0;
}
} // libcis_ns_localization_locales_locale_convenience_classification_isprint_7deac00c (libcis)

