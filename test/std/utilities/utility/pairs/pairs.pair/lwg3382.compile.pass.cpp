// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_pairs_pairs_pair_lwg3382).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <utility>

namespace libcis_ns_utilities_utility_pairs_pairs_pair_lwg3382 { // libcis: isolate file-scope helpers
template <auto>
struct Test {};

void test() {
  // LWG 3382. NTTP for pair and array
  // https://cplusplus.github.io/LWG/issue3382
#if !defined(_LIBCPP_DEPRECATED_ABI_DISABLE_PAIR_TRIVIAL_COPY_CTOR)
  constexpr std::pair<int, long> a{};
  [[maybe_unused]] Test<a> test1{};
#endif
}
} using namespace libcis_ns_utilities_utility_pairs_pairs_pair_lwg3382; // libcis

