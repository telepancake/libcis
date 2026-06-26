// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_cnstr_PR27684_contains_ref_to_incomplete_type_656953e6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03

// <tuple>

// template <class... Types> class tuple;

// template <class Alloc> tuple(allocator_arg_t, Alloc const&)

// See https://llvm.org/PR27684.

#include <tuple>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_cnstr_PR27684_contains_ref_to_incomplete_type_656953e6 { // libcis
struct IncompleteType;
extern IncompleteType inc1;
extern IncompleteType inc2;
IncompleteType const& cinc1 = inc1;
IncompleteType const& cinc2 = inc2;

int main(int, char**) {
    using IT = IncompleteType;
    { // try calling tuple(Tp const&...)
        using Tup = std::tuple<const IT&, const IT&>;
        Tup t(cinc1, cinc2);
        assert(&std::get<0>(t) == &inc1);
        assert(&std::get<1>(t) == &inc2);
    }
    { // try calling tuple(Up&&...)
        using Tup = std::tuple<const IT&, const IT&>;
        Tup t(inc1, inc2);
        assert(&std::get<0>(t) == &inc1);
        assert(&std::get<1>(t) == &inc2);
    }

  return 0;

    return 0;
}

struct IncompleteType {};
IncompleteType inc1;
IncompleteType inc2;
} // libcis_ns_utilities_tuple_tuple_tuple_tuple_cnstr_PR27684_contains_ref_to_incomplete_type_656953e6 (libcis)

