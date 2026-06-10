// AST-transferred from libc++ by tools/transfer.py (slug=thread_macro).
// main -> test_thread_macro; file-scope helpers isolated in anon namespace.
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

// #define __STDCPP_THREADS__ 1

#include <thread>

#include "test_macros.h"

void test_thread_macro()
{
#ifndef __STDCPP_THREADS__
#error __STDCPP_THREADS__ is not defined
#elif __STDCPP_THREADS__ != 1
#error __STDCPP_THREADS__ has the wrong value
#endif

  return;
}
