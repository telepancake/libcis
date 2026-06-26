// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_list_incomplete_type_d84aabf8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// Check that std::list and its iterators can be instantiated with an incomplete
// type.

#include <list>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_list_incomplete_type_d84aabf8 { // libcis
struct A {
    std::list<A> l;
    std::list<A>::iterator it;
    std::list<A>::const_iterator cit;
    std::list<A>::reverse_iterator rit;
    std::list<A>::const_reverse_iterator crit;
};

int main(int, char**) {
    A a;

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_list_incomplete_type_d84aabf8 (libcis)

