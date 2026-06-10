// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostream_format_input_streams_istream_unformatted_seekg).
// main -> test_input_output_iostream_format_input_streams_istream_unformatted_seekg; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <istream>

// basic_istream<charT,traits>& seekg(pos_type pos);

#include <istream>
#include <cassert>
#include <streambuf>

#include "test_macros.h"

namespace libcis_ns_input_output_iostream_format_input_streams_istream_unformatted_seekg { // libcis: isolate file-scope helpers
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

    CharT* eback() const {return base::eback();}
    CharT* gptr() const {return base::gptr();}
    CharT* egptr() const {return base::egptr();}
protected:
    typename base::pos_type seekpos(typename base::pos_type sp,
                                    std::ios_base::openmode which)
    {
        assert(which == std::ios_base::in);
        return sp;
    }
};
} using namespace libcis_ns_input_output_iostream_format_input_streams_istream_unformatted_seekg; // libcis


void test_input_output_iostream_format_input_streams_istream_unformatted_seekg()
{
    {
        testbuf<char> sb(" 123456789");
        std::istream is(&sb);
        is.seekg(5);
        assert(is.good());
        is.seekg(-1);
        assert(is.fail());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        testbuf<wchar_t> sb(L" 123456789");
        std::wistream is(&sb);
        is.seekg(5);
        assert(is.good());
        is.seekg(-1);
        assert(is.fail());
    }
#endif
    {
        testbuf<char> sb(" 123456789");
        std::istream is(&sb);
        is.setstate(std::ios_base::eofbit);
        assert(is.eof());
        is.seekg(5);
        assert(is.good());
        assert(!is.eof());
    }

  return;
}
