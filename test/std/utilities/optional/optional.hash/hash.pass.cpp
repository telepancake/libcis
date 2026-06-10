// AST-transferred from libc++ by tools/transfer.py (slug=utilities_optional_optional_hash_hash).
// main -> test_utilities_optional_optional_hash_hash; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// template <class T> struct hash<optional<T>>;

#include <optional>
#include <string>
#include <memory>
#include <cassert>

#include "poisoned_hash_helper.h"

#include "test_macros.h"

namespace libcis_ns_utilities_optional_optional_hash_hash { // libcis: isolate file-scope helpers
struct A {};
struct B {};
} using namespace libcis_ns_utilities_optional_optional_hash_hash; // libcis


template <>
struct std::hash<B> {
  std::size_t operator()(B const&) noexcept(false) { return 0; }
};

void test_utilities_optional_optional_hash_hash()
{
    using std::optional;
    const std::size_t nullopt_hash =
        std::hash<optional<double>>{}(optional<double>{});


    {
        optional<B> opt;
        ASSERT_NOT_NOEXCEPT(std::hash<optional<B>>()(opt));
        ASSERT_NOT_NOEXCEPT(std::hash<optional<const B>>()(opt));
    }

    {
        typedef int T;
        optional<T> opt;
        assert(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = 2;
        assert(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
        typedef std::string T;
        optional<T> opt;
        assert(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = std::string("123");
        assert(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
        typedef std::unique_ptr<int> T;
        optional<T> opt;
        assert(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = std::unique_ptr<int>(new int(3));
        assert(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
      test_hash_enabled<std::optional<int> >();
      test_hash_enabled<std::optional<int*> >();
      test_hash_enabled<std::optional<const int> >();
      test_hash_enabled<std::optional<int* const> >();

      test_hash_disabled<std::optional<A>>();
      test_hash_disabled<std::optional<const A>>();

      test_hash_enabled<std::optional<B>>();
      test_hash_enabled<std::optional<const B>>();
    }

  return;
}
