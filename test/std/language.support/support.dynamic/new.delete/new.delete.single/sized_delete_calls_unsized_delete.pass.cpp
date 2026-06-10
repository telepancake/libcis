// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_dynamic_new_delete_new_delete_single_sized_delete_calls_unsized_delete).
// main -> test_language_support_support_dynamic_new_delete_new_delete_single_sized_delete_calls_unsized_delete; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test sized operator delete calls the unsized operator delete.
// When sized operator delete is not available (ex C++11) then the unsized
// operator delete is called directly.

// UNSUPPORTED: sanitizer-new-delete

#include <new>
#include <cstddef>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_new_delete_new_delete_single_sized_delete_calls_unsized_delete { // libcis: isolate file-scope helpers
int delete_called = 0;
int delete_nothrow_called = 0;

void operator delete(void* p) TEST_NOEXCEPT
{
    ++delete_called;
    std::free(p);
}

void operator delete(void* p, const std::nothrow_t&) TEST_NOEXCEPT
{
    ++delete_nothrow_called;
    std::free(p);
}
} using namespace libcis_ns_language_support_support_dynamic_new_delete_new_delete_single_sized_delete_calls_unsized_delete; // libcis


void test_language_support_support_dynamic_new_delete_new_delete_single_sized_delete_calls_unsized_delete()
{
    int *x = new int(42);
    DoNotOptimize(x);
    assert(0 == delete_called);
    assert(0 == delete_nothrow_called);

    delete x;
    DoNotOptimize(x);
    assert(1 == delete_called);
    assert(0 == delete_nothrow_called);

  return;
}
