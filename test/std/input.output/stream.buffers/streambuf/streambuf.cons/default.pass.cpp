// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_stream_buffers_streambuf_streambuf_cons_default_98896186).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8

// <streambuf>

// template <class charT, class traits = char_traits<charT> >
// class basic_streambuf;

// basic_streambuf();

#include <streambuf>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

namespace libcis_ns_input_output_stream_buffers_streambuf_streambuf_cons_default_98896186 { // libcis
template <class CharT>
struct test
    : public std::basic_streambuf<CharT>
{
    test()
    {
        assert(this->eback() == 0);
        assert(this->gptr() == 0);
        assert(this->egptr() == 0);
        assert(this->pbase() == 0);
        assert(this->pptr() == 0);
        assert(this->epptr() == 0);
    }
};

int main(int, char**)
{
    {
        test<char> t;
        assert(t.getloc().name() == "C");
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        test<wchar_t> t;
        assert(t.getloc().name() == "C");
    }
#endif

    std::locale::global(std::locale(LOCALE_en_US_UTF_8));
    {
        test<char> t;
        assert(t.getloc().name() == LOCALE_en_US_UTF_8);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        test<wchar_t> t;
        assert(t.getloc().name() == LOCALE_en_US_UTF_8);
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_stream_buffers_streambuf_streambuf_cons_default_98896186 (libcis)

