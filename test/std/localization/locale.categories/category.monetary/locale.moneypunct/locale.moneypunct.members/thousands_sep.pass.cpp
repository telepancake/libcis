// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_monetary_locale_moneypunct_locale_moneypunct_members_thousands_sep).
// main -> test_localization_locale_categories_category_monetary_locale_moneypunct_locale_moneypunct_members_thousands_sep; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class moneypunct<charT, International>

// charT thousands_sep() const;

// The C++ and C standards are silent.
//   POSIX standard is being followed (as a guideline).

#include <locale>
#include <limits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_monetary_locale_moneypunct_locale_moneypunct_members_thousands_sep { // libcis: isolate file-scope helpers
typedef std::moneypunct<char> F;

class Fnf
    : public std::moneypunct<char, false>
{
public:
    explicit Fnf(std::size_t refs = 0)
        : std::moneypunct<char, false>(refs) {}
};

class Fnt
    : public std::moneypunct<char, true>
{
public:
    explicit Fnt(std::size_t refs = 0)
        : std::moneypunct<char, true>(refs) {}
};

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
class Fwf
    : public std::moneypunct<wchar_t, false>
{
public:
    explicit Fwf(std::size_t refs = 0)
        : std::moneypunct<wchar_t, false>(refs) {}
};

class Fwt
    : public std::moneypunct<wchar_t, true>
{
public:
    explicit Fwt(std::size_t refs = 0)
        : std::moneypunct<wchar_t, true>(refs) {}
};
} using namespace libcis_ns_localization_locale_categories_category_monetary_locale_moneypunct_locale_moneypunct_members_thousands_sep; // libcis

#endif // TEST_HAS_NO_WIDE_CHARACTERS

void test_localization_locale_categories_category_monetary_locale_moneypunct_locale_moneypunct_members_thousands_sep()
{
    {
        Fnf f(1);
        assert(f.thousands_sep() == std::numeric_limits<char>::max());
    }
    {
        Fnt f(1);
        assert(f.thousands_sep() == std::numeric_limits<char>::max());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        Fwf f(1);
        assert(f.thousands_sep() == std::numeric_limits<wchar_t>::max());
    }
    {
        Fwt f(1);
        assert(f.thousands_sep() == std::numeric_limits<wchar_t>::max());
    }
#endif // TEST_HAS_NO_WIDE_CHARACTERS

  return;
}
