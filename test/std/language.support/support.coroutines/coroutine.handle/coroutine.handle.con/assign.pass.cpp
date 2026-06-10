// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_coroutines_coroutine_handle_coroutine_handle_con_assign).
// main -> test_language_support_support_coroutines_coroutine_handle_coroutine_handle_con_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <coroutine>

// template <class Promise = void>
// struct coroutine_handle;

// coroutine_handle& operator=(nullptr_t) noexcept

#include <coroutine>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_language_support_support_coroutines_coroutine_handle_coroutine_handle_con_assign { // libcis: isolate file-scope helpers
template <class C>
void do_test() {
  int dummy = 42;
  void* dummy_h = &dummy;
  {
    static_assert(std::is_nothrow_assignable<C&, std::nullptr_t>::value, "");
    static_assert(!std::is_assignable<C&, void*>::value, "");
  }
  {
    C c = C::from_address(dummy_h);
    assert(c.address() == &dummy);
    c = nullptr;
    assert(c.address() == nullptr);
    c = nullptr;
    assert(c.address() == nullptr);
  }
  {
    C c;
    C& cr = (c = nullptr);
    assert(&c == &cr);
  }
}
} using namespace libcis_ns_language_support_support_coroutines_coroutine_handle_coroutine_handle_con_assign; // libcis


void test_language_support_support_coroutines_coroutine_handle_coroutine_handle_con_assign()
{
  do_test<std::coroutine_handle<>>();
  do_test<std::coroutine_handle<int>>();

  return;
}
