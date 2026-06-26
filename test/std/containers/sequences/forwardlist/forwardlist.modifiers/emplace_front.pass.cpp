// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_forwardlist_forwardlist_modifiers_emplace_front_dfd44d29).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <forward_list>

// template <class... Args> reference emplace_front(Args&&... args);
// return type is 'reference' in C++17; 'void' before

#include <forward_list>
#include <cassert>

#include "test_macros.h"

#include "../../../Emplaceable.h"
#include "min_allocator.h"

namespace libcis_ns_containers_sequences_forwardlist_forwardlist_modifiers_emplace_front_dfd44d29 { // libcis
int main(int, char**)
{
    {
        typedef Emplaceable T;
        typedef std::forward_list<T> C;
        C c;
#if TEST_STD_VER > 14
        T& r1 = c.emplace_front();
        assert(c.front() == Emplaceable());
        assert(&r1 == &c.front());
        assert(std::distance(c.begin(), c.end()) == 1);
        T& r2 = c.emplace_front(1, 2.5);
        assert(c.front() == Emplaceable(1, 2.5));
        assert(&r2 == &c.front());
#else
        c.emplace_front();
        assert(c.front() == Emplaceable());
        assert(std::distance(c.begin(), c.end()) == 1);
        c.emplace_front(1, 2.5);
        assert(c.front() == Emplaceable(1, 2.5));
#endif
        assert(*std::next(c.begin()) == Emplaceable());
        assert(std::distance(c.begin(), c.end()) == 2);
    }
    {
        typedef Emplaceable T;
        typedef std::forward_list<T, min_allocator<T>> C;
        C c;
#if TEST_STD_VER > 14
        T& r1 = c.emplace_front();
        assert(c.front() == Emplaceable());
        assert(&r1 == &c.front());
        assert(std::distance(c.begin(), c.end()) == 1);
        T& r2 = c.emplace_front(1, 2.5);
        assert(c.front() == Emplaceable(1, 2.5));
        assert(&r2 == &c.front());
#else
        c.emplace_front();
        assert(c.front() == Emplaceable());
        assert(std::distance(c.begin(), c.end()) == 1);
        c.emplace_front(1, 2.5);
        assert(c.front() == Emplaceable(1, 2.5));
#endif
        assert(*std::next(c.begin()) == Emplaceable());
        assert(std::distance(c.begin(), c.end()) == 2);
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_forwardlist_forwardlist_modifiers_emplace_front_dfd44d29 (libcis)

