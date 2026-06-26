// transferred+adapted from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_observers_key_comp_fecd11a1).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// key_compare key_comp() const;

#include <map>
#include <cassert>
#include <string>

namespace libcis_ns_containers_associative_multimap_multimap_observers_key_comp_fecd11a1 { // libcis
int main(int, char**) {
    typedef std::multimap<int, std::string> map_type;

    map_type m;
    map_type::iterator i1 = m.insert(map_type::value_type(1, "abc"));
    map_type::iterator i2 = m.insert(map_type::value_type(2, "abc"));

    const map_type& cm = m;

    assert(cm.key_comp()(i1->first, i2->first));
    assert(!cm.key_comp()(i2->first, i1->first));

    return 0;

    return 0;
}
} // libcis_ns_containers_associative_multimap_multimap_observers_key_comp_fecd11a1 (libcis)

