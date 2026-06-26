// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_conversions_conversions_string_ctor_codecvt_state_723f9b88).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_CODECVT -D_LIBCPP_ENABLE_CXX26_REMOVED_WSTRING_CONVERT

// wstring_convert<Codecvt, Elem, Wide_alloc, Byte_alloc>

// wstring_convert(Codecvt* pcvt, state_type state);

// XFAIL: no-wide-characters

#include <locale>
#include <codecvt>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_convenience_conversions_conversions_string_ctor_codecvt_state_723f9b88 { // libcis
int main(int, char**)
{
    {
        typedef std::codecvt_utf8<wchar_t> Codecvt;
        typedef std::wstring_convert<Codecvt> Myconv;
        Myconv myconv(new Codecvt, std::mbstate_t());
        assert(myconv.converted() == 0);
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locales_locale_convenience_conversions_conversions_string_ctor_codecvt_state_723f9b88 (libcis)

