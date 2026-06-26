// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_optional_optional_object_optional_object_observe_bool_a8e98955).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr explicit optional<T>::operator bool() const noexcept;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_optional_optional_object_optional_object_observe_bool_a8e98955 { // libcis
int main(int, char**)
{
    using std::optional;
    {
        const optional<int> opt; ((void)opt);
        ASSERT_NOEXCEPT(bool(opt));
        static_assert(!std::is_convertible<optional<int>, bool>::value, "");
    }
    {
        constexpr optional<int> opt;
        static_assert(!opt, "");
    }
    {
        constexpr optional<int> opt(0);
        static_assert(opt, "");
    }

  return 0;

    return 0;
}
} // libcis_ns_utilities_optional_optional_object_optional_object_observe_bool_a8e98955 (libcis)

