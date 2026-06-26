// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_file_streams_fstreams_filebuf_cons_default_3608cb32).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <fstream>

// template <class charT, class traits = char_traits<charT> >
// class basic_filebuf

// basic_filebuf();

#include <fstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_file_streams_fstreams_filebuf_cons_default_3608cb32 { // libcis
int main(int, char**)
{
    {
        std::filebuf f;
        assert(!f.is_open());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wfilebuf f;
        assert(!f.is_open());
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_file_streams_fstreams_filebuf_cons_default_3608cb32 (libcis)

