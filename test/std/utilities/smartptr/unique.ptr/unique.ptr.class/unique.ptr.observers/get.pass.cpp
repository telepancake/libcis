// AST-transferred from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get).
// main -> test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// unique_ptr

// pointer unique_ptr<T>::get() const noexcept;
// pointer unique_ptr<T[]>::get() const noexcept;

#include <memory>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get { // libcis: isolate file-scope helpers
template <class T>
TEST_CONSTEXPR_CXX23 void test_basic() {
  // non-const element type
  {
    // non-const access
    {
      T* x = new T;
      std::unique_ptr<T> ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T*);
      ASSERT_NOEXCEPT(ptr.get());
      assert(ptr.get() == x);
    }

    // const access
    {
      T* x = new T;
      std::unique_ptr<T> const ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T*);
      ASSERT_NOEXCEPT(ptr.get());
      assert(ptr.get() == x);
    }
  }

  // const element type
  {
    // non-const access
    {
      T* x = new T;
      std::unique_ptr<T const> ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T const*);
      assert(ptr.get() == x);
    }

    // const access
    {
      T* x = new T;
      std::unique_ptr<T const> const ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T const*);
      assert(ptr.get() == x);
    }
  }

  // Same thing but for unique_ptr<T[]>
  // non-const element type
  {
    // non-const access
    {
      T* x = new T[3];
      std::unique_ptr<T[]> ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T*);
      ASSERT_NOEXCEPT(ptr.get());
      assert(ptr.get() == x);
    }

    // const access
    {
      T* x = new T[3];
      std::unique_ptr<T[]> const ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T*);
      ASSERT_NOEXCEPT(ptr.get());
      assert(ptr.get() == x);
    }
  }

  // const element type
  {
    // non-const access
    {
      T* x = new T[3];
      std::unique_ptr<T const[]> ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T const*);
      assert(ptr.get() == x);
    }

    // const access
    {
      T* x = new T[3];
      std::unique_ptr<T const[]> const ptr(x);
      ASSERT_SAME_TYPE(decltype(ptr.get()), T const*);
      assert(ptr.get() == x);
    }
  }
}

template <std::size_t Size>
struct WithSize {
  char padding[Size];
};

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic<char>();
  test_basic<int>();
  test_basic<WithSize<1> >();
  test_basic<WithSize<2> >();
  test_basic<WithSize<3> >();
  test_basic<WithSize<4> >();
  test_basic<WithSize<8> >();
  test_basic<WithSize<16> >();
  test_basic<WithSize<256> >();

  return true;
}
} using namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get; // libcis


void test_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get() {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return;
}
