// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_syncstream_syncbuf_syncstream_syncbuf_special_swap_e9dd5946).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-localization
// UNSUPPORTED: libcpp-has-no-experimental-syncstream

// <syncstream>

//  template<class charT, class traits, class Allocator>
//    void swap(basic_syncbuf<charT, traits, Allocator>&,
//              basic_syncbuf<charT, traits, Allocator>&);

#include <syncstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_special_swap_e9dd5946 { // libcis
template <class CharT>
void test() {
  std::basic_syncbuf<CharT> base1;
  std::basic_syncbuf<CharT> base2;
  std::basic_syncbuf<CharT> buff1(&base1);
  std::basic_syncbuf<CharT> buff2(&base2);
  std::swap(buff1, buff2);

  assert(buff1.get_wrapped() == &base2);
  assert(buff2.get_wrapped() == &base1);
}

int main(int, char**) {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_special_swap_e9dd5946 (libcis)

