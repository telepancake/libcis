// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_lwg3629).
// main -> test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_lwg3629; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_condition

// template <ErrorCodeEnum E> error_condition& operator=(E e);

// Regression test for https://llvm.org/PR57614

namespace libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_lwg3629 { // libcis: isolate file-scope helpers
int make_error_condition;
} using namespace libcis_ns_diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_lwg3629; // libcis
 // It's important that this comes before <system_error>

#include <system_error>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace User {
  enum Err {};

  std::error_condition make_error_condition(Err) { return std::error_condition(42, std::generic_category()); }
}

template <>
struct std::is_error_condition_enum<User::Err> : true_type {};

void test_diagnostics_syserr_syserr_errcondition_syserr_errcondition_modifiers_lwg3629() {
  std::error_condition e;
  e = User::Err();
  assert(e.value() == 42);
  assert(e.category() == std::generic_category());
  return;
}
