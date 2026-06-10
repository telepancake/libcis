// AST-transferred from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_fstream_assign_move_assign).
// main -> test_input_output_file_streams_fstreams_fstream_assign_move_assign; file-scope helpers isolated in anon namespace.
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

// basic_fstream& operator=(basic_fstream&& rhs);

#include <fstream>
#include <cassert>
#include "test_macros.h"
#include "platform_support.h"

void test_input_output_file_streams_fstreams_fstream_assign_move_assign()
{
    std::string temp = get_temp_file_name();
    {
        std::fstream fso(temp.c_str(), std::ios_base::in | std::ios_base::out
                                                 | std::ios_base::trunc);
        std::fstream fs;
        fs = std::move(fso);
        double x = 0;
        fs << 3.25;
        fs.seekg(0);
        fs >> x;
        assert(x == 3.25);
    }
    std::remove(temp.c_str());

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wfstream fso(temp.c_str(), std::ios_base::in | std::ios_base::out
                                                  | std::ios_base::trunc);
        std::wfstream fs;
        fs = std::move(fso);
        double x = 0;
        fs << 3.25;
        fs.seekg(0);
        fs >> x;
        assert(x == 3.25);
    }
    std::remove(temp.c_str());
#endif

  return;
}
