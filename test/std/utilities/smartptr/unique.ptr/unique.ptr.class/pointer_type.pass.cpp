// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_class_pointer_type).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_class_pointer_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// unique_ptr

// Test unique_ptr::pointer type

#include <memory>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_pointer_type { // libcis: isolate file-scope helpers
struct Deleter {
  struct pointer {};
};

struct D2 {
private:
  typedef void pointer;
};

struct D3 {
  static long pointer;
};

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  {
    typedef std::unique_ptr<VT> P;
    static_assert((std::is_same<typename P::pointer, int*>::value), "");
  }
  {
    typedef std::unique_ptr<VT, Deleter> P;
    static_assert((std::is_same<typename P::pointer, Deleter::pointer>::value),
                  "");
  }
#if TEST_STD_VER >= 11
  {
    typedef std::unique_ptr<VT, D2> P;
    static_assert(std::is_same<typename P::pointer, int*>::value, "");
  }
  {
    typedef std::unique_ptr<VT, D3> P;
    static_assert(std::is_same<typename P::pointer, int*>::value, "");
  }
#endif
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_pointer_type; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_class_pointer_type() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
