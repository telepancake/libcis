// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_dynamic_nothrow_t_6753ba19).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// struct nothrow_t {
//   explicit nothrow_t() = default;
// };
// extern const nothrow_t nothrow;

#include <new>


namespace libcis_ns_language_support_support_dynamic_nothrow_t_6753ba19 { // libcis
int main(int, char**) {
  std::nothrow_t x = std::nothrow;
  (void)x;

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_dynamic_nothrow_t_6753ba19 (libcis)

