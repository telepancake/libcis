// AST-transferred from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_fstream_members_rdbuf).
// main -> test_input_output_file_streams_fstreams_fstream_members_rdbuf; file-scope helpers isolated in anon namespace.
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

// basic_filebuf<charT,traits>* rdbuf() const;

#include <fstream>
#include <cassert>

#include "test_macros.h"

void test_input_output_file_streams_fstreams_fstream_members_rdbuf()
{
    {
        std::fstream fs;
        assert(fs.rdbuf());
    }

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wfstream fs;
        assert(fs.rdbuf());
    }
#endif

  return;
}
