// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_runtime_csetjmp).
// main -> test_language_support_support_runtime_csetjmp; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// MSVC warning C4611: interaction between '_setjmp' and C++ object destruction is non-portable
// ADDITIONAL_COMPILE_FLAGS(cl-style-warnings): /wd4611

// test <csetjmp>

#include <csetjmp>
#include <cassert>
#include <type_traits>

void test_language_support_support_runtime_csetjmp() {
  std::jmp_buf jb;

  switch (setjmp(jb)) {
  // First time we set the buffer, the function should return 0
  case 0:
    break;

  // If it returned 42, then we're coming from the std::longjmp call below
  case 42:
    return;

  // Otherwise, something is wrong
  default:
    return;
  }

  std::longjmp(jb, 42);
  static_assert(std::is_same<decltype(std::longjmp(jb, 0)), void>::value, "");

  return;
}
