// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multimap_incomplete_type_cf5004cd).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// Check that std::multimap and its iterators can be instantiated with an incomplete
// type.

#include <map>

#include "test_macros.h"

namespace libcis_ns_containers_associative_multimap_incomplete_type_cf5004cd { // libcis
struct A {
    typedef std::multimap<A, A> Map;
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
} // libcis_ns_containers_associative_multimap_incomplete_type_cf5004cd (libcis)

