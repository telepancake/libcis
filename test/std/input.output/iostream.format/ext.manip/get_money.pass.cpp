// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_ext_manip_get_money).
// main -> test_input_output_iostream_format_ext_manip_get_money; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iomanip>

// template <class moneyT> T7 get_money(moneyT& mon, bool intl = false);

// Bionic has minimal locale support, investigate this later.
// XFAIL: LIBCXX-ANDROID-FIXME

// REQUIRES: locale.en_US.UTF-8

#include <cassert>
#include <iomanip>
#include <istream>
#include <streambuf>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

namespace libcis_ns_input_output_iostream_format_ext_manip_get_money { // libcis: isolate file-scope helpers
template <class CharT>
struct testbuf
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_string<CharT> string_type;
    typedef std::basic_streambuf<CharT> base;
private:
    string_type str_;
public:

    testbuf() {}
    testbuf(const string_type& str)
        : str_(str)
    {
        base::setg(const_cast<CharT*>(str_.data()),
                   const_cast<CharT*>(str_.data()),
                   const_cast<CharT*>(str_.data()) + str_.size());
    }
};
} using namespace libcis_ns_input_output_iostream_format_ext_manip_get_money; // libcis


void test_input_output_iostream_format_ext_manip_get_money()
{
    {
#if defined(_WIN32)
        testbuf<char> sb("  ($1,234,567.89)");
#else
        testbuf<char> sb("  -$1,234,567.89");
#endif
        std::istream is(&sb);
        is.imbue(std::locale(LOCALE_en_US_UTF_8));
        long double x = 0;
        is >> std::get_money(x, false);
        assert(x == -123456789);
    }
    {
#if defined(_WIN32)
        testbuf<char> sb("  (USD 1,234,567.89)");
#else
        testbuf<char> sb("  -USD 1,234,567.89");
#endif
        std::istream is(&sb);
        is.imbue(std::locale(LOCALE_en_US_UTF_8));
        long double x = 0;
        is >> std::get_money(x, true);
        assert(x == -123456789);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
#if defined(_WIN32)
        testbuf<wchar_t> sb(L"  ($1,234,567.89)");
#else
        testbuf<wchar_t> sb(L"  -$1,234,567.89");
#endif
        std::wistream is(&sb);
        is.imbue(std::locale(LOCALE_en_US_UTF_8));
        long double x = 0;
        is >> std::get_money(x, false);
        assert(x == -123456789);
    }
    {
#if defined(_WIN32)
        testbuf<wchar_t> sb(L"  (USD 1,234,567.89)");
#else
        testbuf<wchar_t> sb(L"  -USD 1,234,567.89");
#endif
        std::wistream is(&sb);
        is.imbue(std::locale(LOCALE_en_US_UTF_8));
        long double x = 0;
        is >> std::get_money(x, true);
        assert(x == -123456789);
    }
#endif

  return;
}
