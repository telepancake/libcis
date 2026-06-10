// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_c_headers_support_c_headers_other_math_lerp).
// main -> test_language_support_support_c_headers_support_c_headers_other_math_lerp; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <math.h>

// [support.c.headers.other]/1
//   ... except for the functions described in [sf.cmath], the
//   std::lerp function overloads ([c.math.lerp]) ...

#include <cassert>
#include <math.h>

namespace libcis_ns_language_support_support_c_headers_support_c_headers_other_math_lerp { // libcis: isolate file-scope helpers
template <class = int>
int lerp(float, float, float) {
  return 32;
}

template <class = int>
int lerp(double, double, double) {
  return 32;
}

template <class = int>
int lerp(long double, long double, long double) {
  return 32;
}
} using namespace libcis_ns_language_support_support_c_headers_support_c_headers_other_math_lerp; // libcis


void test_language_support_support_c_headers_support_c_headers_other_math_lerp() {
  assert(lerp(0.f, 0.f, 0.f) == 32);
  assert(lerp(0., 0., 0.) == 32);
  assert(lerp(0.l, 0.l, 0.l) == 32);

  return;
}
