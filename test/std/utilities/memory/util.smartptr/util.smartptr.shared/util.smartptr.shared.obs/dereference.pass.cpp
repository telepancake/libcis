// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_dereference).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_dereference; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// T& operator*() const;

#include <memory>
#include <cassert>

#include "test_macros.h"

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_dereference()
{
    const std::shared_ptr<int> p(new int(32));
    assert(*p == 32);
    *p = 3;
    assert(*p == 3);

  return;
}
