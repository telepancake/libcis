// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr).
// main -> test_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class T, class D>
// struct hash<unique_ptr<T, D>>
// {
//     typedef unique_ptr<T, D> argument_type;
//     typedef size_t           result_type;
//     size_t operator()(const unique_ptr<T, D>& p) const;
// };

#include <memory>

#include <cassert>
#include <functional>

#include "test_macros.h"

#if TEST_STD_VER >= 11
#include "poisoned_hash_helper.h"
#include "deleter_types.h"
#include "min_allocator.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr { // libcis: isolate file-scope helpers
template <class ValueT, class Del>
void test_enabled_with_deleter() {
  using UPtr = std::unique_ptr<ValueT, Del>;
  using pointer = typename UPtr::pointer;
  using RawDel = typename std::decay<Del>::type;
  RawDel d(1);
  UPtr p(nullptr, std::forward<Del>(d));
  test_hash_enabled<UPtr>(p);
  test_hash_enabled<pointer>();
}

template <class ValueT, class Del>
void test_disabled_with_deleter() {
  using UPtr = std::unique_ptr<ValueT, Del>;
  using pointer = typename UPtr::pointer;
  test_hash_disabled<UPtr>();
  test_hash_disabled<pointer>();
}
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr; // libcis


template <class T>
struct std::hash<min_pointer<T, std::integral_constant<std::size_t, 1>>> {
  std::size_t operator()(min_pointer<T, std::integral_constant<size_t, 1>> p) const TEST_NOEXCEPT_FALSE {
    if (!p) return 0;
    return std::hash<T*>{}(std::addressof(*p));
  }
};

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr { // libcis: isolate file-scope helpers
struct A {};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr; // libcis


#endif // TEST_STD_VER >= 11

void test_utilities_memory_util_smartptr_util_smartptr_hash_hash_unique_ptr()
{
  {
    int* ptr = new int;
    std::unique_ptr<int> p(ptr);
    std::hash<std::unique_ptr<int> > f;
    std::size_t h = f(p);
    assert(h == std::hash<int*>()(ptr));
  }
#if TEST_STD_VER >= 11
  {
    std::unique_ptr<int, PointerDeleter<int, 1>> pThrowingHash;
    std::hash<std::unique_ptr<int, PointerDeleter<int, 1>>> fThrowingHash;
    ASSERT_NOT_NOEXCEPT(fThrowingHash(pThrowingHash));
  }
  {
    test_enabled_with_deleter<int, Deleter<int>>();
    test_enabled_with_deleter<int[], Deleter<int[]>>();
    test_enabled_with_deleter<int, CopyDeleter<int>>();
    test_enabled_with_deleter<int, CopyDeleter<int[]>>();
    test_enabled_with_deleter<int, NCDeleter<int>&>();
    test_enabled_with_deleter<int[], NCDeleter<int[]>&>();
    test_enabled_with_deleter<int, NCConstDeleter<int> const&>();
    test_enabled_with_deleter<int[], NCConstDeleter<int[]> const&>();
  }
  {
    test_enabled_with_deleter<int, PointerDeleter<int, 1>>();
    test_enabled_with_deleter<int[], PointerDeleter<int[], 1>>();
    test_enabled_with_deleter<A, PointerDeleter<A, 1>>();
    test_enabled_with_deleter<A[], PointerDeleter<A[], 1>>();

    test_disabled_with_deleter<int, PointerDeleter<int, 0>>();
    test_disabled_with_deleter<int[], PointerDeleter<int[], 0>>();
    test_disabled_with_deleter<A, PointerDeleter<A, 0>>();
    test_disabled_with_deleter<A[], PointerDeleter<A[], 0>>();
  }
#endif

  return;
}
