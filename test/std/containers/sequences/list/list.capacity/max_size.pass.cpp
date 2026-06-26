// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_list_capacity_max_size_44a44e11).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// size_type max_size() const noexcept

#include <cassert>
#include <limits>
#include <list>
#include <type_traits>

#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_containers_sequences_list_list_capacity_max_size_44a44e11 { // libcis
int main(int, char**) {
  {
    typedef limited_allocator<int, 10> A;
    typedef std::list<int, A> C;
    C c;
    assert(c.max_size() <= 10);
    LIBCPP_ASSERT(c.max_size() == 10);
  }
  {
    typedef limited_allocator<int, (std::size_t)-1> A;
    typedef std::list<int, A> C;
    const C::size_type max_dist =
        static_cast<C::size_type>(std::numeric_limits<C::difference_type>::max());
    C c;
    assert(c.max_size() <= max_dist);
    LIBCPP_ASSERT(c.max_size() == max_dist);
  }
  {
    typedef std::list<char> C;
    const C::size_type max_dist =
        static_cast<C::size_type>(std::numeric_limits<C::difference_type>::max());
    C c;
    assert(c.max_size() <= max_dist);
    assert(c.max_size() <= alloc_max_size(c.get_allocator()));
  }

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_list_capacity_max_size_44a44e11 (libcis)

