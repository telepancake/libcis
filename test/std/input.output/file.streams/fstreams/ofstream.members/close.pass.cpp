// AST-transferred from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_ofstream_members_close).
// main -> test_input_output_file_streams_fstreams_ofstream_members_close; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <fstream>

// template <class charT, class traits = char_traits<charT> >
// class basic_ofstream

// void close();

#include <fstream>
#include <cassert>
#include "test_macros.h"
#include "platform_support.h"

void test_input_output_file_streams_fstreams_ofstream_members_close()
{
    std::string temp = get_temp_file_name();
    {
        std::ofstream fs;
        assert(!fs.is_open());
        fs.open(temp.c_str());
        assert(fs.is_open());
        fs.close();
        assert(!fs.is_open());
    }
    std::remove(temp.c_str());

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wofstream fs;
        assert(!fs.is_open());
        fs.open(temp.c_str());
        assert(fs.is_open());
        fs.close();
        assert(!fs.is_open());
    }
    std::remove(temp.c_str());
#endif

  return;
}
