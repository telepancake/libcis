// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_map_map_observers_value_comp_f452817a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// value_compare value_comp() const;

#include <map>
#include <cassert>
#include <string>

namespace libcis_ns_containers_associative_map_map_observers_value_comp_f452817a { // libcis
int main(int, char**) {
    typedef std::map<int, std::string> map_type;

    map_type m;
    std::pair<map_type::iterator, bool> p1 = m.insert(map_type::value_type(1, "abc"));
    std::pair<map_type::iterator, bool> p2 = m.insert(map_type::value_type(2, "abc"));

    const map_type& cm = m;

    assert(cm.value_comp()(*p1.first, *p2.first));
    assert(!cm.value_comp()(*p2.first, *p1.first));

    return 0;

    return 0;
}
} // libcis_ns_containers_associative_map_map_observers_value_comp_f452817a (libcis)

