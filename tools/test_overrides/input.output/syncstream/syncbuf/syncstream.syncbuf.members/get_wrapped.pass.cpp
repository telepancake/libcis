// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_syncstream_syncbuf_syncstream_syncbuf_members_get_wrapped).
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

// streambuf_type* get_wrapped() const noexcept;

#include <syncstream>
#include <concepts>
#include <cassert>

#include "../helpers.h"
#include "test_macros.h"

namespace libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_members_get_wrapped { // libcis
template <class T>
void test() {
  test_buf<T> base(42);
  const std::allocator<T> alloc;
  const test_syncbuf<T> buff(&base, alloc);
  auto wrapped = buff.get_wrapped();
  // libcis: gcc-10 constrained-placeholder defect (dependent constraint
  // arg); proven -- type check kept as static_assert.
  static_assert(std::is_same_v<decltype(wrapped), std::basic_streambuf<T>*>);
  assert(static_cast<test_buf<T>*>(wrapped)->id == 42);
  ASSERT_NOEXCEPT(buff.get_wrapped());
}

int main(int, char**) {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return 0;
}
} // libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_members_get_wrapped (libcis)

