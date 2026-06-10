// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_id_thread_id).
// main -> test_thread_thread_threads_thread_thread_class_thread_thread_id_thread_id; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <thread>

// template <class T>
// struct hash
// {
//     size_t operator()(T val) const;
// };

// Not very portable

#include <cassert>
#include <functional>
#include <thread>

#include "test_macros.h"

void test_thread_thread_threads_thread_thread_class_thread_thread_id_thread_id()
{
    std::thread::id id1;
    std::thread::id id2 = std::this_thread::get_id();
    typedef std::hash<std::thread::id> H;
#if TEST_STD_VER <= 14
    static_assert((std::is_same<typename H::argument_type, std::thread::id>::value), "" );
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "" );
#endif
    ASSERT_NOEXCEPT(H()(id2));
    H h;
    assert(h(id1) != h(id2));

  return;
}
