// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl).
// main -> test_thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03

// <thread>

// class thread

// template <class F, class ...Args> thread(F&& f, Args&&... args);

#include <thread>

#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl { // libcis: isolate file-scope helpers
struct Incomplete;
template<class T> struct Holder { T t; };

void f(Holder<Incomplete> *) { }
} using namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl; // libcis


void test_thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl()
{
    Holder<Incomplete> *p = nullptr;
    std::thread t(f, p);
    t.join();
    return;
}
