// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_syncstream_syncbuf_syncstream_syncbuf_members_set_emit_on_sync_4281490d).
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

// void set_emit_on_sync(bool) noexcept;

#include <syncstream>
#include <cassert>

#include "test_macros.h"
#include "../helpers.h"

namespace libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_members_set_emit_on_sync_4281490d { // libcis
template <class T>
void test_set_emit_on_sync() {
  // set_emit_on_sync tested in sync, which is called by pubsync. The assign
  // and swap test use this.
  test_syncbuf<T, std::allocator<T>> buff(nullptr, std::allocator<T>());
  ASSERT_NOEXCEPT(buff.set_emit_on_sync(false));
  buff.set_emit_on_sync(false); // Validates the function can be called.
}

int main(int, char**) {
  test_set_emit_on_sync<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test_set_emit_on_sync<wchar_t>();
#endif

  return 0;

    return 0;
}
} // libcis_ns_input_output_syncstream_syncbuf_syncstream_syncbuf_members_set_emit_on_sync_4281490d (libcis)

