// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_new_delete_new_delete_placement_new_5806afd8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test placement new

#include <new>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_new_delete_new_delete_placement_new_5806afd8 { // libcis
int A_constructed = 0;

struct A {
  A() { ++A_constructed; }
  ~A() { --A_constructed; }
};

TEST_CONSTEXPR_OPERATOR_NEW void test_direct_call() {
  assert(::operator new(sizeof(int), &A_constructed) == &A_constructed);

  char ch = '*';
  assert(::operator new(1, &ch) == &ch);
  assert(ch == '*');
}

#ifdef __cpp_lib_constexpr_new
static_assert((test_direct_call(), true));
#endif

int main(int, char**) {
  char buf[sizeof(A)];

  A* ap = new (buf) A;
  assert((char*)ap == buf);
  assert(A_constructed == 1);

  test_direct_call();
  return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_new_delete_new_delete_placement_new_5806afd8 (libcis)

