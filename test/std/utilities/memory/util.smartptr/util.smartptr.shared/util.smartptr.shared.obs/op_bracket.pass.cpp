// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_op_bracket_0b2da956).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// UNSUPPORTED: c++03, c++11, c++14

// shared_ptr

// element_type& operator[](ptrdiff_t i) const;

#include "test_macros.h"

#include <memory>
#include <cassert>

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_op_bracket_0b2da956 { // libcis
int main(int, char**) {
  {
    const std::shared_ptr<int[8]> p(new int[8]);

    for (int i = 0; i < 8; ++i)
      p[i] = i;
    for (int i = 0; i < 8; ++i)
      assert(p[i] == i);
  }
  {
    int* iptr = new int[8];
    for (int i = 0; i < 8; ++i)
      iptr[i] = i;

    const std::shared_ptr<int[8]> p(iptr);

    for (int i = 0; i < 8; ++i)
      assert(p[i] == i);
  }
  {
    const std::shared_ptr<int[]> p(new int[8]);

    for (int i = 0; i < 8; ++i)
      p[i] = i;
    for (int i = 0; i < 8; ++i)
      assert(p[i] == i);
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_op_bracket_0b2da956 (libcis)

