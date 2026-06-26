// transferred+adapted from libc++ by tools/transfer.py (slug=experimental_simd_simd_mask_class_simd_mask_width_c79da8c6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <experimental/simd>
//
// [simd.mask.class]
// static constexpr std::size_t size() noexcept;

#include "../test_utils.h"

namespace ex = std::experimental::parallelism_v2;

namespace libcis_ns_experimental_simd_simd_mask_class_simd_mask_width_c79da8c6 { // libcis
template <class T, std::size_t>
struct CheckSimdMaskWidth {
  template <class SimdAbi>
  void operator()() {
    static_assert(ex::simd_mask<T, SimdAbi>::size() == ex::simd_size_v<T, SimdAbi>);
  }
};

int main(int, char**) {
  test_all_simd_abi<CheckSimdMaskWidth>();
  return 0;

    return 0;
}
} // libcis_ns_experimental_simd_simd_mask_class_simd_mask_width_c79da8c6 (libcis)

