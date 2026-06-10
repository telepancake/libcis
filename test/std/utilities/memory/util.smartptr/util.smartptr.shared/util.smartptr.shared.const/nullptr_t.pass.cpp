// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t; file-scope helpers isolated in anon namespace.
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

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t()
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

  return;
}
