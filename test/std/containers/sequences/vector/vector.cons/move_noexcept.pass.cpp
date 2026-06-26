// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_vector_cons_move_noexcept_f485976f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// vector(vector&&)
//        noexcept(is_nothrow_move_constructible<allocator_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

namespace libcis_ns_containers_sequences_vector_vector_cons_move_noexcept_f485976f { // libcis
template <class T>
struct some_alloc
{
    typedef T value_type;
    some_alloc(const some_alloc&);
    void allocate(std::size_t);
};

int main(int, char**)
{
    {
        typedef std::vector<MoveOnly> C;
        static_assert(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef std::vector<MoveOnly, test_allocator<MoveOnly>> C;
        static_assert(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef std::vector<MoveOnly, other_allocator<MoveOnly>> C;
        static_assert(std::is_nothrow_move_constructible<C>::value, "");
    }
    {
        typedef std::vector<MoveOnly, some_alloc<MoveOnly>> C;
    //  In C++17, move constructors for allocators are not allowed to throw
#if TEST_STD_VER > 14
        static_assert( std::is_nothrow_move_constructible<C>::value, "");
#else
        static_assert(!std::is_nothrow_move_constructible<C>::value, "");
#endif
    }

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_vector_vector_cons_move_noexcept_f485976f (libcis)

