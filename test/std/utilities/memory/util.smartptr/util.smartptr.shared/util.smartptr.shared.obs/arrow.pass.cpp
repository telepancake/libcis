// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow; file-scope helpers isolated in anon namespace.
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

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_obs_arrow()
{
    const std::shared_ptr<std::pair<int, int> > p(new std::pair<int, int>(3, 4));
    assert(p->first == 3);
    assert(p->second == 4);
    p->first = 5;
    p->second = 6;
    assert(p->first == 5);
    assert(p->second == 6);

  return;
}
