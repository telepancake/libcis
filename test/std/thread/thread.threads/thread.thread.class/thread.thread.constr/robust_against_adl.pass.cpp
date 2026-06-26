// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl_e8203c89).
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

namespace libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl_e8203c89 { // libcis
struct Incomplete;
template<class T> struct Holder { T t; };

void f(Holder<Incomplete> *) { }

int main(int, char **)
{
    Holder<Incomplete> *p = nullptr;
    std::thread t(f, p);
    t.join();
    return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_class_thread_thread_constr_robust_against_adl_e8203c89 (libcis)

