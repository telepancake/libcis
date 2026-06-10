// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_hash_hash_shared_ptr).
// main -> test_utilities_memory_util_smartptr_util_smartptr_hash_hash_shared_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class T>
// struct hash<shared_ptr<T>>
// {
//     typedef shared_ptr<T>    argument_type;
//     typedef size_t           result_type;
//     size_t operator()(const shared_ptr<T>& p) const;
// };

#include <memory>

#include <cassert>
#include <functional>

#include "test_macros.h"

#if TEST_STD_VER >= 11
#include "poisoned_hash_helper.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_hash_hash_shared_ptr { // libcis: isolate file-scope helpers
struct A {};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_hash_hash_shared_ptr; // libcis

#endif

void test_utilities_memory_util_smartptr_util_smartptr_hash_hash_shared_ptr()
{
  {
    int* ptr = new int;
    std::shared_ptr<int> p(ptr);
    std::hash<std::shared_ptr<int> > f;
    std::size_t h = f(p);
    assert(h == std::hash<int*>()(ptr));
  }
#if TEST_STD_VER >= 11
  {
    test_hash_enabled<std::shared_ptr<int>>();
    test_hash_enabled<std::shared_ptr<A>>();
  }
#endif

  return;
}
