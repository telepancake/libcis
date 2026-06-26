// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_capacity_empty_95877437).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// class vector

// bool empty() const noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_vector_vector_capacity_empty_95877437 { // libcis
TEST_CONSTEXPR_CXX20 bool tests() {
    {
    typedef std::vector<int> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    assert(c.empty());
    c.push_back(C::value_type(1));
    assert(!c.empty());
    c.clear();
    assert(c.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::vector<int, min_allocator<int>> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    assert(c.empty());
    c.push_back(C::value_type(1));
    assert(!c.empty());
    c.clear();
    assert(c.empty());
    }
    {
      typedef std::vector<int, safe_allocator<int>> C;
      C c;
      ASSERT_NOEXCEPT(c.empty());
      assert(c.empty());
      c.push_back(C::value_type(1));
      assert(!c.empty());
      c.clear();
      assert(c.empty());
    }
#endif

    return true;
}

int main(int, char**)
{
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;

    return 0;
}
} // libcis_ns_containers_sequences_vector_vector_capacity_empty_95877437 (libcis)

