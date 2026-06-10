// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_specialized_algorithms_specialized_addressof_addressof).
// main -> test_utilities_memory_specialized_algorithms_specialized_addressof_addressof; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// template <ObjectType T> T* addressof(T& r);

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_specialized_algorithms_specialized_addressof_addressof { // libcis: isolate file-scope helpers
struct A
{
    void operator&() const {}
};

struct nothing {
    operator char&()
    {
        static char c;
        return c;
    }
};
} using namespace libcis_ns_utilities_memory_specialized_algorithms_specialized_addressof_addressof; // libcis


void test_utilities_memory_specialized_algorithms_specialized_addressof_addressof()
{
    {
    int i;
    double d;
    assert(std::addressof(i) == &i);
    assert(std::addressof(d) == &d);
    A* tp = new A;
    const A* ctp = tp;
    assert(std::addressof(*tp) == tp);
    assert(std::addressof(*ctp) == tp);
    delete tp;
    }
    {
    union
    {
        nothing n;
        int i;
    };
    assert(std::addressof(n) == (void*)std::addressof(i));
    }

  return;
}
