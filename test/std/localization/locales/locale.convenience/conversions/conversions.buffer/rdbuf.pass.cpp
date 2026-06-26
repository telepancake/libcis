// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locales_locale_convenience_conversions_conversions_buffer_rdbuf_440234c0).
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

// streambuf *rdbuf(streambuf *bytebuf);

// XFAIL: no-wide-characters

#include <locale>
#include <codecvt>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_convenience_conversions_conversions_buffer_rdbuf_440234c0 { // libcis
int main(int, char**)
{
    typedef std::wbuffer_convert<std::codecvt_utf8<wchar_t> > B;
    {
        std::stringstream s;
        B b;
        assert(b.rdbuf() == nullptr);
        b.rdbuf(s.rdbuf());
        assert(b.rdbuf() == s.rdbuf());
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locales_locale_convenience_conversions_conversions_buffer_rdbuf_440234c0 (libcis)

