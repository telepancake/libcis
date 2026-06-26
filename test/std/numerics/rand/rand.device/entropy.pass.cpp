// transferred+adapted from libc++ by tools/transfer.py (slug=numerics_rand_rand_device_entropy_93e782cd).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-random-device

// <random>

// class random_device;

// double entropy() const;

#include <random>
#include <cassert>
#include <climits>

#include "test_macros.h"

namespace libcis_ns_numerics_rand_rand_device_entropy_93e782cd { // libcis
int main(int, char**) {
  std::random_device r;
  double e = r.entropy();
  assert(e >= 0);
  assert(e <= sizeof(typename std::random_device::result_type) * CHAR_BIT);

  return 0;

    return 0;
}
} // libcis_ns_numerics_rand_rand_device_entropy_93e782cd (libcis)

