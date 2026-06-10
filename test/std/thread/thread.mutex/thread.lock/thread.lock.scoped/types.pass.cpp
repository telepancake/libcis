// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_scoped_types).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_scoped_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14

// <mutex>

// template <class... MutexTypes>
// class scoped_lock
// {
// public:
//     typedef Mutex mutex_type;  // Only if sizeof...(MutexTypes) == 1
//     ...
// };

#include <mutex>
#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_scoped_types { // libcis: isolate file-scope helpers
struct NAT {};

template <class LG>
auto test_typedef(int) -> typename LG::mutex_type;

template <class LG>
auto test_typedef(...) -> NAT;

template <class LG>
constexpr bool has_mutex_type() {
    return !std::is_same<decltype(test_typedef<LG>(0)), NAT>::value;
}
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_scoped_types; // libcis


void test_thread_thread_mutex_thread_lock_thread_lock_scoped_types()
{
    {
        using T = std::scoped_lock<>;
        static_assert(!has_mutex_type<T>(), "");
    }
    {
        using M1 = std::mutex;
        using T = std::scoped_lock<M1>;
        static_assert(std::is_same<T::mutex_type, M1>::value, "");
    }
    {
        using M1 = std::recursive_mutex;
        using T = std::scoped_lock<M1>;
        static_assert(std::is_same<T::mutex_type, M1>::value, "");
    }
    {
        using M1 = std::mutex;
        using M2 = std::recursive_mutex;
        using T = std::scoped_lock<M1, M2>;
        static_assert(!has_mutex_type<T>(), "");
    }
    {
        using M1 = std::mutex;
        using M2 = std::recursive_mutex;
        using T = std::scoped_lock<M1, M1, M2>;
        static_assert(!has_mutex_type<T>(), "");
    }
    {
        using M1 = std::mutex;
        using T = std::scoped_lock<M1, M1>;
        static_assert(!has_mutex_type<T>(), "");
    }
    {
        using M1 = std::recursive_mutex;
        using T = std::scoped_lock<M1, M1, M1>;
        static_assert(!has_mutex_type<T>(), "");
    }

  return;
}
