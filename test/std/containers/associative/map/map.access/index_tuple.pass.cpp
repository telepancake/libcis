// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_map_access_index_tuple_38edf2d6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <map>

// class map

// mapped_type& operator[](const key_type& k);

// https://llvm.org/PR16542

#include <map>
#include <tuple>

namespace libcis_ns_containers_associative_map_map_access_index_tuple_38edf2d6 { // libcis
int main(int, char**) {
    std::map<std::tuple<int, int>, std::size_t> m;
    m[std::make_tuple(2, 3)] = 7;

    return 0;

    return 0;
}
} // libcis_ns_containers_associative_map_map_access_index_tuple_38edf2d6 (libcis)

