// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_fstream_cons_default_ac937bcb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <fstream>

// template <class charT, class traits = char_traits<charT> >
// class basic_fstream

// basic_fstream();

// XFAIL: FROZEN-CXX03-HEADERS-FIXME

#include <fstream>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_input_output_file_streams_fstreams_fstream_cons_default_ac937bcb { // libcis
int main(int, char**)
{
    {
        std::fstream fs;
    }
    {
      std::basic_fstream<char, operator_hijacker_char_traits<char> > fs;
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wfstream fs;
    }
    {
      std::basic_fstream<wchar_t, operator_hijacker_char_traits<wchar_t> > fs;
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_file_streams_fstreams_fstream_cons_default_ac937bcb (libcis)

