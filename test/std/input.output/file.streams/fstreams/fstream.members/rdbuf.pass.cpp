// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_fstream_members_rdbuf_cf1b11d3).
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

namespace libcis_ns_input_output_file_streams_fstreams_fstream_members_rdbuf_cf1b11d3 { // libcis
int main(int, char**)
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

  return 0;

    return 0;
}
} // libcis_ns_input_output_file_streams_fstreams_fstream_members_rdbuf_cf1b11d3 (libcis)

