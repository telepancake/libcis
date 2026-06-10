// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_output_streams_ostream_unformatted_write).
// main -> test_input_output_iostream_format_output_streams_ostream_unformatted_write; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ostream>

// template <class charT, class traits = char_traits<charT> >
//   class basic_ostream;

// basic_ostream& write(const char_type* s, streamsize n);

#include <ostream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_output_streams_ostream_unformatted_write { // libcis: isolate file-scope helpers
template <class CharT>
class testbuf
    : public std::basic_streambuf<CharT>
{
    typedef std::basic_streambuf<CharT> base;
    std::basic_string<CharT> str_;
public:
    testbuf()
    {
    }

    std::basic_string<CharT> str() const
        {return std::basic_string<CharT>(base::pbase(), base::pptr());}

protected:

    virtual typename base::int_type
        overflow(typename base::int_type ch = base::traits_type::eof())
        {
            if (ch != base::traits_type::eof())
            {
                int n = static_cast<int>(str_.size());
                str_.push_back(static_cast<CharT>(ch));
                str_.resize(str_.capacity());
                base::setp(const_cast<CharT*>(str_.data()),
                           const_cast<CharT*>(str_.data() + str_.size()));
                base::pbump(n+1);
            }
            return ch;
        }
};
} using namespace libcis_ns_input_output_iostream_format_output_streams_ostream_unformatted_write; // libcis


void test_input_output_iostream_format_output_streams_ostream_unformatted_write()
{
    {
        std::ostream os((std::streambuf*)0);
        const char s[] = "123456790";
        os.write(s, sizeof(s)/sizeof(s[0])-1);
        assert(os.bad());
    }
    {
        testbuf<char> sb;
        std::ostream os(&sb);
        const char s[] = "123456790";
        os.write(s, sizeof(s)/sizeof(s[0])-1);
        assert(sb.str() == s);
        assert(os.good());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wostream os((std::wstreambuf*)0);
        const wchar_t s[] = L"123456790";
        os.write(s, sizeof(s)/sizeof(s[0])-1);
        assert(os.bad());
    }
    {
        testbuf<wchar_t> sb;
        std::wostream os(&sb);
        const wchar_t s[] = L"123456790";
        os.write(s, sizeof(s)/sizeof(s[0])-1);
        assert(os.good());
        assert(sb.str() == s);
    }
#endif

  return;
}
