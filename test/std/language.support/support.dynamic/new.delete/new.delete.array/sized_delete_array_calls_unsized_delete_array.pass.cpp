// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_dynamic_new_delete_new_delete_array_sized_delete_array_calls_unsized_delete_array).
// main -> test_language_support_support_dynamic_new_delete_new_delete_array_sized_delete_array_calls_unsized_delete_array; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test sized operator [] delete calls the unsized operator [] delete.
// When sized operator delete [] is not available (ex C++11) then the unsized
// operator delete [] is called directly.

// UNSUPPORTED: sanitizer-new-delete

#include <new>
#include <cstddef>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_new_delete_new_delete_array_sized_delete_array_calls_unsized_delete_array { // libcis: isolate file-scope helpers
int delete_called = 0;
int delete_nothrow_called = 0;

void operator delete[](void* p) TEST_NOEXCEPT
{
    ++delete_called;
    std::free(p);
}

void operator delete[](void* p, const std::nothrow_t&) TEST_NOEXCEPT
{
    ++delete_nothrow_called;
    std::free(p);
}

// NOTE: Use a class with a non-trivial destructor as the test type in order
// to ensure the correct overload is called.
// C++14 5.3.5 [expr.delete]p10
// - If the type is complete and if, for the second alternative (delete array)
//   only, the operand is a pointer to a class type with a non-trivial
//   destructor or a (possibly multi-dimensional) array thereof, the function
//   with two parameters is selected.
// - Otherwise, it is unspecified which of the two deallocation functions is
//   selected.
struct A { ~A() {} };
} using namespace libcis_ns_language_support_support_dynamic_new_delete_new_delete_array_sized_delete_array_calls_unsized_delete_array; // libcis


void test_language_support_support_dynamic_new_delete_new_delete_array_sized_delete_array_calls_unsized_delete_array()
{
    A *x = new A[3];
    DoNotOptimize(x);
    assert(0 == delete_called);
    assert(0 == delete_nothrow_called);

    delete [] x;
    DoNotOptimize(x);
    assert(1 == delete_called);
    assert(0 == delete_nothrow_called);

  return;
}
