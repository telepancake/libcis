// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_optional_optional_object_optional_object_observe_op_arrow_260600cf).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr T* optional<T>::operator->();

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_optional_optional_object_optional_object_observe_op_arrow_260600cf { // libcis
using std::optional;

struct X
{
    int test() noexcept {return 3;}
};

struct Y
{
    constexpr int test() {return 3;}
};

constexpr int
test()
{
    optional<Y> opt{Y{}};
    return opt->test();
}

int main(int, char**)
{
    {
        std::optional<X> opt; ((void)opt);
        ASSERT_SAME_TYPE(decltype(opt.operator->()), X*);
        ASSERT_NOEXCEPT(opt.operator->());
    }
    {
        optional<X> opt(X{});
        assert(opt->test() == 3);
    }
    {
        static_assert(test() == 3, "");
    }

    return 0;

    return 0;
}
} // libcis_ns_utilities_optional_optional_object_optional_object_observe_op_arrow_260600cf (libcis)

