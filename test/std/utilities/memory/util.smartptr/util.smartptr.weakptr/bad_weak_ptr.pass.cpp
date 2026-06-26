// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weakptr_bad_weak_ptr_38d6666b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// class bad_weak_ptr
//     : public std::exception
// {
// public:
//     bad_weak_ptr();
// };

#include <exception>
#include <memory>
#include <type_traits>
#include <cassert>
#include <cstring>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weakptr_bad_weak_ptr_38d6666b { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::exception, std::bad_weak_ptr>::value), "");
    std::bad_weak_ptr e;
    std::bad_weak_ptr e2 = e;
    e2 = e;
    assert(std::strcmp(e.what(), "bad_weak_ptr") == 0);

  return 0;

    return 0;
}
} // libcis_ns_utilities_memory_util_smartptr_util_smartptr_weakptr_bad_weak_ptr_38d6666b (libcis)

