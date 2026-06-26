// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_allocator_adaptor_allocator_adaptor_members_max_size_0d039f22).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// size_type max_size() const;

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"

namespace libcis_ns_utilities_allocator_adaptor_allocator_adaptor_members_max_size_0d039f22 { // libcis
int main(int, char**) {
  {
    typedef std::scoped_allocator_adaptor<A1<int>> A;
    const A a(A1<int>(100));
    assert(a.max_size() == 100);
  }
  {
    typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
    const A a(A1<int>(20), A2<int>());
    assert(a.max_size() == 20);
  }
  {
    typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
    const A a(A1<int>(200), A2<int>(), A3<int>());
    assert(a.max_size() == 200);
  }

  return 0;

    return 0;
}
} // libcis_ns_utilities_allocator_adaptor_allocator_adaptor_members_max_size_0d039f22 (libcis)

