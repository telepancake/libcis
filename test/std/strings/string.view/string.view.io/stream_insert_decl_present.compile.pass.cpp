// AST-transferred from libc++ by tools/transfer.py (slug=strings_string_view_string_view_io_stream_insert_decl_present).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: !stdlib=libc++ && (c++03 || c++11 || c++14)

// <string_view>

// template<class charT, class traits, class Allocator>
//   basic_ostream<charT, traits>&
//   operator<<(basic_ostream<charT, traits>& os,
//              const basic_string_view<charT,traits> str);

#include <iosfwd>
#include <string_view>
#include <utility>

namespace libcis_ns_strings_string_view_string_view_io_stream_insert_decl_present { // libcis: isolate file-scope helpers
template <class SV, class = void>
struct HasDecl : std::false_type {};
template <class SV>
struct HasDecl<SV, decltype(static_cast<void>(std::declval<std::ostream&>() << std::declval<SV&>()))> : std::true_type {
};

static_assert(HasDecl<std::string_view>::value, "streaming operator declaration not present");
} using namespace libcis_ns_strings_string_view_string_view_io_stream_insert_decl_present; // libcis

