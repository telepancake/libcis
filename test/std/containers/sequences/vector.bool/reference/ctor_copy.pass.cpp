// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_vector_bool_reference_ctor_copy_527aad88).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// reference(const reference&)

#include <cassert>
#include <vector>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_vector_bool_reference_ctor_copy_527aad88 { // libcis
TEST_CONSTEXPR_CXX20 bool test() {
  std::vector<bool> vec;
  typedef std::vector<bool>::reference Ref;
  vec.push_back(true);
  Ref ref = vec[0];
  Ref ref2 = ref;
  assert(ref == ref2 && ref2);
  ref.flip();
  assert(ref == ref2 && !ref2);

  return true;
}

int main(int, char**) {
  test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_vector_bool_reference_ctor_copy_527aad88 (libcis)

