// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_string_streams_stringstream_stringstream_cons_move2_ad1a42a5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
// class basic_stringstream

// basic_stringstream(basic_stringstream&& rhs);

#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_input_output_string_streams_stringstream_stringstream_cons_move2_ad1a42a5 { // libcis
int main(int, char**)
{
  std::vector<std::stringstream> vecss;
  vecss.push_back(std::stringstream());
  vecss.back().str("hub started at [00 6b 8b 45 69]");
  vecss.push_back(std::stringstream());
  vecss.back().str("hub started at [00 6b 8b 45 69]");
  for (std::size_t n = 0; n < vecss.size(); n++) {
    assert(vecss[n].str().size() == 31);
    vecss[n].seekg(0, std::ios_base::beg);
    assert(vecss[n].str().size() == 31);
  }

  return 0;

    return 0;
}
} // libcis_ns_input_output_string_streams_stringstream_stringstream_cons_move2_ad1a42a5 (libcis)

