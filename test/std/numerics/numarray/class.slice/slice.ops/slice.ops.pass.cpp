// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_numarray_class_slice_slice_ops_slice_ops_7b664358).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <valarray>

// class slice;

// friend bool operator==(const slice& x, const slice& y);

#include <cassert>
#include <valarray>

#include "test_comparisons.h"

namespace libcis_ns_numerics_numarray_class_slice_slice_ops_slice_ops_7b664358 { // libcis
void test() {
  {
    std::slice s1;
    std::slice s2;

    assert(testEquality(s1, s2, true));
  }
  {
    std::slice s1{1, 2, 3};
    std::slice s2{1, 2, 3};

    assert(testEquality(s1, s2, true));
  }
  {
    std::slice s1;
    std::slice s2{1, 2, 3};

    assert(testEquality(s1, s2, false));
  }
  {
    std::slice s1{0, 2, 3};
    std::slice s2{1, 2, 3};

    assert(testEquality(s1, s2, false));
  }
  {
    std::slice s1{1, 0, 3};
    std::slice s2{1, 2, 3};

    assert(testEquality(s1, s2, false));
  }
  {
    std::slice s1{1, 2, 0};
    std::slice s2{1, 2, 3};

    assert(testEquality(s1, s2, false));
  }
}

int main(int, char**) {
  test();

  return 0;

    return 0;
}
} // libcis_ns_numerics_numarray_class_slice_slice_ops_slice_ops_7b664358 (libcis)

