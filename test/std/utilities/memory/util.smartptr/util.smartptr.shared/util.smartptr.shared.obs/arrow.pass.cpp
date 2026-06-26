// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow_ea4704e7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// T* operator->() const;

#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow_ea4704e7 { // libcis
int main(int, char**)
{
    const std::shared_ptr<std::pair<int, int> > p(new std::pair<int, int>(3, 4));
    assert(p->first == 3);
    assert(p->second == 4);
    p->first = 5;
    p->second = 6;
    assert(p->first == 5);
    assert(p->second == 6);

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow_ea4704e7 (libcis)

