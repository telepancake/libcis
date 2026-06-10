// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_default_allocator_allocator_globals_eq).
// main -> test_utilities_memory_default_allocator_allocator_globals_eq; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// allocator:

// template <class T1, class T2>
//   constexpr bool
//   operator==(const allocator<T1>&, const allocator<T2>&) throw();
//
// template <class T1, class T2>
//   constexpr bool
//   operator!=(const allocator<T1>&, const allocator<T2>&) throw();

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_default_allocator_allocator_globals_eq { // libcis: isolate file-scope helpers
TEST_CONSTEXPR_CXX20 bool test()
{
    std::allocator<int> a1;
    std::allocator<int> a2;
    assert(a1 == a2);
    assert(!(a1 != a2));

    return true;
}
} using namespace libcis_ns_utilities_memory_default_allocator_allocator_globals_eq; // libcis


void test_utilities_memory_default_allocator_allocator_globals_eq()
{
    test();

#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return;
}
