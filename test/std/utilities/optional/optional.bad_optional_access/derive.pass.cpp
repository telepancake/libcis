// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_optional_optional_bad_optional_access_derive_c054ff9b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <optional>

// class bad_optional_access : public exception

#include <optional>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_optional_optional_bad_optional_access_derive_c054ff9b { // libcis
int main(int, char**)
{
    using std::bad_optional_access;

    static_assert(std::is_base_of<std::exception, bad_optional_access>::value, "");
    static_assert(std::is_convertible<bad_optional_access*, std::exception*>::value, "");

  return 0;

    return 0;
}
} // libcis_ns_utilities_optional_optional_bad_optional_access_derive_c054ff9b (libcis)

