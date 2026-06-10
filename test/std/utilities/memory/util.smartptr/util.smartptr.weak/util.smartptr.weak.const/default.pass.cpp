// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_default).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// template<class T> class weak_ptr

// weak_ptr();

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_default { // libcis: isolate file-scope helpers
struct A;
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_default; // libcis


void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_default()
{
    std::weak_ptr<A> p;
    assert(p.use_count() == 0);

  return;
}
