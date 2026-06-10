// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_move_assign_noexcept).
// main -> test_containers_sequences_vector_bool_move_assign_noexcept; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// vector& operator=(vector&& c)
//     noexcept(
//          allocator_type::propagate_on_container_move_assignment::value &&
//          is_nothrow_move_assignable<allocator_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <cassert>
#include <vector>
#include <type_traits>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_vector_bool_move_assign_noexcept { // libcis: isolate file-scope helpers
template <class T>
struct some_alloc {
  typedef T value_type;
  some_alloc(const some_alloc&);
};

template <class T>
struct some_alloc2 {
  typedef T value_type;

  some_alloc2() {}
  some_alloc2(const some_alloc2&);
  void deallocate(void*, unsigned) {}

  typedef std::false_type propagate_on_container_move_assignment;
  typedef std::true_type is_always_equal;
};

template <class T>
struct some_alloc3 {
  typedef T value_type;

  some_alloc3() {}
  some_alloc3(const some_alloc3&);
  void deallocate(void*, unsigned) {}

  typedef std::false_type propagate_on_container_move_assignment;
  typedef std::false_type is_always_equal;
};
} using namespace libcis_ns_containers_sequences_vector_bool_move_assign_noexcept; // libcis


void test_containers_sequences_vector_bool_move_assign_noexcept() {
#if defined(_LIBCPP_VERSION)
  {
    typedef std::vector<bool> C;
    static_assert(std::is_nothrow_move_assignable<C>::value, "");
  }
#endif // _LIBCPP_VERSION
  {
    typedef std::vector<bool, test_allocator<bool>> C;
    static_assert(!std::is_nothrow_move_assignable<C>::value, "");
  }
#if defined(_LIBCPP_VERSION)
  {
    typedef std::vector<bool, other_allocator<bool>> C;
    static_assert(std::is_nothrow_move_assignable<C>::value, "");
  }
#endif // _LIBCPP_VERSION
  {
#if TEST_STD_VER > 14
#  if defined(_LIBCPP_VERSION)
    typedef std::vector<bool, some_alloc<bool>> C;
    static_assert(std::is_nothrow_move_assignable<C>::value, "");
#  endif // _LIBCPP_VERSION
#else
    typedef std::vector<bool, some_alloc<bool>> C;
    static_assert(!std::is_nothrow_move_assignable<C>::value, "");
#endif
  }
#if TEST_STD_VER > 14
#  if defined(_LIBCPP_VERSION)
  { // POCMA false, is_always_equal true
    typedef std::vector<bool, some_alloc2<bool>> C;
    static_assert(std::is_nothrow_move_assignable<C>::value, "");
  }
#  endif // _LIBCPP_VERSION
  {      // POCMA false, is_always_equal false
    typedef std::vector<bool, some_alloc3<bool>> C;
    static_assert(!std::is_nothrow_move_assignable<C>::value, "");
  }
#endif

  return;
}
