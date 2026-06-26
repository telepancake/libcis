// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_sizezero_d6c3edc3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// This code triggers https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104568
// UNSUPPORTED: msvc

// Test the fix for https://llvm.org/PR54100

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_sizezero_d6c3edc3 { // libcis
struct A {
  int m[0];
};
static_assert(sizeof(A) == 0, "");  // an extension supported by GCC and Clang

int main(int, char**)
{
  {
    std::unique_ptr<A> p = std::unique_ptr<A>(new A);
    assert(p != nullptr);
  }
  {
    std::unique_ptr<A[]> p = std::unique_ptr<A[]>(new A[1]);
    assert(p != nullptr);
  }
#if TEST_STD_VER > 11
  {
    std::unique_ptr<A> p = std::make_unique<A>();
    assert(p != nullptr);
  }
  {
    std::unique_ptr<A[]> p = std::make_unique<A[]>(1);
    assert(p != nullptr);
  }
#endif

  return 0;

    return 0;
}
} // libcis_ns_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_sizezero_d6c3edc3 (libcis)

