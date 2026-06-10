// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_future_error_types).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <future>

// class future_error : public logic_error {...};

#include <future>
#include <type_traits>

namespace libcis_ns_thread_futures_futures_future_error_types { // libcis: isolate file-scope helpers
static_assert(std::is_convertible<std::future_error*, std::logic_error*>::value, "");
} using namespace libcis_ns_thread_futures_futures_future_error_types; // libcis

