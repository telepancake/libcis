// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_incomplete_type_1d0794c0).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// Check that std::map and its iterators can be instantiated with an incomplete
// type.

#include <map>

#include "test_macros.h"

namespace libcis_ns_containers_associative_map_incomplete_type_1d0794c0 { // libcis
struct A {
    typedef std::map<A, A> Map;
    int data;
    Map m;
    Map::iterator it;
    Map::const_iterator cit;
};

inline bool operator==(A const& L, A const& R) { return &L == &R; }
inline bool operator<(A const& L, A const& R)  { return L.data < R.data; }
int main(int, char**) {
    A a;

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_map_incomplete_type_1d0794c0 (libcis)

