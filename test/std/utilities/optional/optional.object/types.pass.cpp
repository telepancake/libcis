// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_optional_optional_object_types_6c9101cb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// template <class T>
// class optional
// {
// public:
//     typedef T value_type;
//     ...

#include <optional>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_optional_optional_object_types_6c9101cb { // libcis
using std::optional;

template <class Opt, class T>
void
test()
{
    static_assert(std::is_same<typename Opt::value_type, T>::value, "");
}

int main(int, char**)
{
    test<optional<int>, int>();
    test<optional<const int>, const int>();
    test<optional<double>, double>();
    test<optional<const double>, const double>();

  return 0;

    return 0;
}
} // libcis_ns_utilities_optional_optional_object_types_6c9101cb (libcis)

