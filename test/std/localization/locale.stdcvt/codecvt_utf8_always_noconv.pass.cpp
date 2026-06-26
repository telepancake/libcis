// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_stdcvt_codecvt_utf8_always_noconv_559a2564).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <codecvt>

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_CODECVT

// template <class Elem, unsigned long Maxcode = 0x10ffff,
//           codecvt_mode Mode = (codecvt_mode)0>
// class codecvt_utf8
//     : public codecvt<Elem, char, mbstate_t>
// {
//     // unspecified
// };

// bool always_noconv() const throw();

#include <codecvt>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_stdcvt_codecvt_utf8_always_noconv_559a2564 { // libcis
int main(int, char**)
{
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef std::codecvt_utf8<wchar_t> C;
        C c;
        bool r = c.always_noconv();
        assert(r == false);
    }
#endif
    {
        typedef std::codecvt_utf8<char16_t> C;
        C c;
        bool r = c.always_noconv();
        assert(r == false);
    }
    {
        typedef std::codecvt_utf8<char32_t> C;
        C c;
        bool r = c.always_noconv();
        assert(r == false);
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_stdcvt_codecvt_utf8_always_noconv_559a2564 (libcis)

