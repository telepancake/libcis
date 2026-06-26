// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_unique_6871c2e9).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// bool unique() const; // deprecated in C++17, removed in C++20

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX20_REMOVED_SHARED_PTR_UNIQUE

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_unique_6871c2e9 { // libcis
int main(int, char**)
{
    const std::shared_ptr<int> p(new int(32));
    assert(p.unique());
    {
    std::shared_ptr<int> p2 = p;
    assert(!p.unique());
    }
    assert(p.unique());

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_unique_6871c2e9 (libcis)

