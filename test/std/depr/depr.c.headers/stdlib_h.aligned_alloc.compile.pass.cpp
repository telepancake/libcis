// AST-transferred from libc++ by tools/transfer.py (slug=depr_depr_c_headers_stdlib_h_aligned_alloc).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test <stdlib.h>
// ::aligned_alloc

// UNSUPPORTED: c++03, c++11, c++14

// ::aligned_alloc is provided by the C library, but it's marked as unavailable
// until macOS 10.15
// XFAIL: target={{.+}}-apple-macosx10.{{(13|14)(.0)?}}

// ::aligned_alloc is not implemented on Windows
// XFAIL: target={{.+}}-windows-{{.+}}

// ::aligned_alloc is available starting with Android P (API 28)
// XFAIL: target={{.+}}-android{{(eabi)?(21|22|23|24|25|26|27)}}

#include <stdlib.h>
#include <type_traits>

namespace libcis_ns_depr_depr_c_headers_stdlib_h_aligned_alloc { // libcis: isolate file-scope helpers
static_assert(std::is_same<decltype(aligned_alloc(1, 0)), void*>::value, "");
} using namespace libcis_ns_depr_depr_c_headers_stdlib_h_aligned_alloc; // libcis

