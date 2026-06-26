// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multimap_scary_13a2361a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class map class multimap

// Extension:  SCARY/N2913 iterator compatibility between map and multimap

#include <map>

#include "test_macros.h"

namespace libcis_ns_containers_associative_multimap_scary_13a2361a { // libcis
int main(int, char**)
{
    typedef std::map<int, int> M1;
    typedef std::multimap<int, int> M2;
    M2::iterator i;
    M1::iterator j = i;
    ((void)j);

  return 0;

    return 0;
}
} // libcis_ns_containers_associative_multimap_scary_13a2361a (libcis)

