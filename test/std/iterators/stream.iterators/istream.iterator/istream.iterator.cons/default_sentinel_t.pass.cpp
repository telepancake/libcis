// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istream_iterator_istream_iterator_cons_default_sentinel_t_c19c4af3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <iterator>
//
// constexpr istream_iterator(default_sentinel_t); // since C++20

#include <iterator>
#include <cassert>

namespace libcis_ns_iterators_stream_iterators_istream_iterator_istream_iterator_cons_default_sentinel_t_c19c4af3 { // libcis
int main(int, char**) {
  using T = std::istream_iterator<int>;

  {
    T it(std::default_sentinel);
    assert(it == T());
  }

  {
    T it = std::default_sentinel;
    assert(it == T());
  }

  {
    constexpr T it(std::default_sentinel);
    (void)it;
  }

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_istream_iterator_istream_iterator_cons_default_sentinel_t_c19c4af3 (libcis)

