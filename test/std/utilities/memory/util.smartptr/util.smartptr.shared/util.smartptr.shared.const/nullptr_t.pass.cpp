// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t_8d3605d7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr(nullptr_t)

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t_8d3605d7 { // libcis
int main(int, char**)
{
  {
    std::shared_ptr<int> p(nullptr);
    assert(p.use_count() == 0);
    assert(p.get() == 0);
  }

  {
    std::shared_ptr<int const> p(nullptr);
    assert(p.use_count() == 0);
    assert(p.get() == 0);
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t_8d3605d7 (libcis)

