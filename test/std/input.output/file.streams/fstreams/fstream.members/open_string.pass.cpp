// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_fstream_members_open_string_889c1446).
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

// void open(const string& s, ios_base::openmode mode = ios_base::in|ios_base::out);

#include <fstream>
#include <cassert>
#include "test_macros.h"
#include "platform_support.h"

namespace libcis_ns_input_output_file_streams_fstreams_fstream_members_open_string_889c1446 { // libcis
int main(int, char**)
{
    std::string temp = get_temp_file_name();
    {
        std::fstream fs;
        assert(!fs.is_open());
        fs.open(temp, std::ios_base::in | std::ios_base::out
                                                     | std::ios_base::trunc);
        assert(fs.is_open());
        double x = 0;
        fs << 3.25;
        fs.seekg(0);
        fs >> x;
        assert(x == 3.25);
    }
    std::remove(temp.c_str());

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wfstream fs;
        assert(!fs.is_open());
        fs.open(temp, std::ios_base::in | std::ios_base::out
                                                     | std::ios_base::trunc);
        assert(fs.is_open());
        double x = 0;
        fs << 3.25;
        fs.seekg(0);
        fs >> x;
        assert(x == 3.25);
    }
    std::remove(temp.c_str());
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_file_streams_fstreams_fstream_members_open_string_889c1446 (libcis)

