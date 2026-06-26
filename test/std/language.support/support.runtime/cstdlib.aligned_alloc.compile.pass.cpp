// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_runtime_cstdlib_aligned_alloc_69ba9cbc).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test <cstdlib>
// std::aligned_alloc

// UNSUPPORTED: c++03, c++11, c++14

// ::aligned_alloc is provided by the C library, but it's marked as unavailable
// until macOS 10.15
// XFAIL: target={{.+}}-apple-macosx10.{{13|14}}

// ::aligned_alloc is not implemented on Windows
// XFAIL: target={{.+}}-windows-{{.+}}

// ::aligned_alloc is available starting with Android P (API 28)
// XFAIL: target={{.+}}-android{{(eabi)?(21|22|23|24|25|26|27)}}

#include <cstdlib>
#include <type_traits>

namespace libcis_ns_language_support_support_runtime_cstdlib_aligned_alloc_69ba9cbc { // libcis
static_assert(std::is_same<decltype(std::aligned_alloc(1, 0)), void*>::value, "");
} // libcis_ns_language_support_support_runtime_cstdlib_aligned_alloc_69ba9cbc (libcis)

