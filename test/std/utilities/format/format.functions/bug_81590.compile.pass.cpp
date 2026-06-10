// AST-transferred from libc++ by tools/transfer.py (slug=utilities_format_format_functions_bug_81590).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// XFAIL: availability-fp_to_chars-missing

// The sample code is based on the bug report
// https://llvm.org/PR81590
//
// Tests whether this formatter does not fail to compile due to nested concept
// evaluation.

#include <format>
#include <variant>

namespace libcis_ns_utilities_format_format_functions_bug_81590 { // libcis: isolate file-scope helpers
struct X : std::variant<X*> {
  X* p = nullptr;
  constexpr const std::variant<X*>& decay() const noexcept { return *this; }
};
} using namespace libcis_ns_utilities_format_format_functions_bug_81590; // libcis


template <>
struct std::formatter<X, char> : std::formatter<std::string, char> {
  static constexpr auto format(const X& x, auto& ctx) {
    if (!x.p)
      return ctx.out();
    auto m = [&](const X* t) { return std::format_to(ctx.out(), "{}", *t); };
    return std::visit(m, x.decay());
  }
};

namespace libcis_ns_utilities_format_format_functions_bug_81590 { // libcis: isolate file-scope helpers
void bug_81590() { (void)std::format("{}", X{}); }
} using namespace libcis_ns_utilities_format_format_functions_bug_81590; // libcis

