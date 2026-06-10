// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_pointer_traits_pointer_traits_types_pointer).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class Ptr>
// struct pointer_traits
// {
//     typedef <details> pointer;
//     ...
// };

#include <memory>
#include <type_traits>

namespace libcis_ns_utilities_memory_pointer_traits_pointer_traits_types_pointer { // libcis: isolate file-scope helpers
struct Foo {
  using element_type = int;
};

static_assert(std::is_same<std::pointer_traits<Foo>::pointer, Foo>::value, "");
} using namespace libcis_ns_utilities_memory_pointer_traits_pointer_traits_types_pointer; // libcis

