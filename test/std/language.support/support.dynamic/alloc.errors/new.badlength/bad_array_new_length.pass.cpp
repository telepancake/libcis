// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_alloc_errors_new_badlength_bad_array_new_length_a0ad0b4e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test bad_array_new_length

#include <new>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_dynamic_alloc_errors_new_badlength_bad_array_new_length_a0ad0b4e { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::bad_alloc, std::bad_array_new_length>::value),
                  "std::is_base_of<std::bad_alloc, std::bad_array_new_length>::value");
    static_assert(std::is_polymorphic<std::bad_array_new_length>::value,
                 "std::is_polymorphic<std::bad_array_new_length>::value");
    std::bad_array_new_length b;
    std::bad_array_new_length b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_alloc_errors_new_badlength_bad_array_new_length_a0ad0b4e (libcis)

