// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_unique_ptr_unique_ptr_special_io_4d25d9e6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03
//  Because we don't have a functioning decltype in C++03

// UNSUPPORTED: no-localization

// <memory>

// unique_ptr

// template<class CharT, class Traits, class Y, class D>
//   basic_ostream<CharT, Traits>&
//   operator<<(basic_ostream<CharT, Traits>& os, const unique_ptr<Y, D>& p);

#include <memory>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_unique_ptr_unique_ptr_special_io_4d25d9e6 { // libcis
int main(int, char**)
{
    std::unique_ptr<int> p(new int(3));
    std::ostringstream os;
    assert(os.str().empty());
    os << p;
    assert(!os.str().empty());

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_unique_ptr_unique_ptr_special_io_4d25d9e6 (libcis)

