// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_explicit_bool).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_explicit_bool; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// unique_ptr

// test constexpr explicit operator bool() const noexcept; // constexpr since C++23

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_explicit_bool { // libcis: isolate file-scope helpers
template <class UPtr>
TEST_CONSTEXPR_CXX23 void doTest(UPtr& p, bool ExpectTrue) {
  if (p)
    assert(ExpectTrue);
  else
    assert(!ExpectTrue);

  if (!p)
    assert(!ExpectTrue);
  else
    assert(ExpectTrue);
}

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  typedef std::unique_ptr<VT> U;
  {
    static_assert((std::is_constructible<bool, U>::value), "");
    static_assert((std::is_constructible<bool, U const&>::value), "");
  }
#if TEST_STD_VER >= 11
  {
    static_assert(!std::is_convertible<U, bool>::value, "");
    static_assert(!std::is_convertible<U const&, bool>::value, "");
  }
#endif
  {
    U p(newValue<VT>(1));
    U const& cp = p;
    doTest(p, true);
    doTest(cp, true);
  }
  {
    U p;
    const U& cp = p;
    doTest(p, false);
    doTest(cp, false);
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_explicit_bool; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_explicit_bool() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}