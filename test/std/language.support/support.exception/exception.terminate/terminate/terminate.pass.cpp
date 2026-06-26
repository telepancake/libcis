// transferred+adapted from libc++ by tools/transfer.py (slug=language_support_support_exception_exception_terminate_terminate_terminate_f607a6b2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test terminate

#include <exception>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_exception_exception_terminate_terminate_terminate_f607a6b2 { // libcis
void f1()
{
    std::exit(0);
}

int main(int, char**)
{
    std::set_terminate(f1);
    std::terminate();
    assert(false);

  return 0;

    return 0;
}
} // libcis_ns_language_support_support_exception_exception_terminate_terminate_terminate_f607a6b2 (libcis)

