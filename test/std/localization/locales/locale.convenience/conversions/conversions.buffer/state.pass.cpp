// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_conversions_conversions_buffer_state_f1fd5456).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_CODECVT -D_LIBCPP_ENABLE_CXX26_REMOVED_WSTRING_CONVERT

// wbuffer_convert<Codecvt, Elem, Tr>

// state_type state() const;

// XFAIL: no-wide-characters

#include <locale>
#include <codecvt>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_convenience_conversions_conversions_buffer_state_f1fd5456 { // libcis
int main(int, char**)
{
    typedef std::wbuffer_convert<std::codecvt_utf8<wchar_t> > B;
    {
        B b;
        std::mbstate_t s = b.state();
        ((void)s);
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locales_locale_convenience_conversions_conversions_buffer_state_f1fd5456 (libcis)

