// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_syncstream_syncbuf_syncstream_syncbuf_cons_dtor_e2f30c36).
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

// template <class charT, class traits, class Allocator>
// class basic_syncbuf;

// ~basic_syncbuf();

#include <syncstream>
#include <cassert>

#include "../helpers.h"
#include "test_macros.h"

namespace libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_cons_dtor_e2f30c36 { // libcis
template <class CharT>
void test() {
  // We do this because we want to be able to use CharT
  CharT arr[3] = {'a', 'b', 'c'};
  CharT* ptr   = arr;

  test_buf<CharT> base;
  const std::allocator<CharT> alloc;
  {
    test_syncbuf<CharT> buf(&base, alloc);

    buf._setp(ptr, ptr + 3);
    assert(base._pptr() == nullptr);
    // The destructor calls buf.emit();
  }
  CharT* pptr = base._pptr();
  while (pptr) {
    assert(*pptr++ == *ptr++);
  }
}

int main(int, char**) {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_cons_dtor_e2f30c36 (libcis)

