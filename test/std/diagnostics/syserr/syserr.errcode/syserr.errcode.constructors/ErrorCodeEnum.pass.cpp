// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum).
// main -> test_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <system_error>

// class error_code

// template <ErrorCodeEnum E> error_code(E e);

#include <system_error>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum { // libcis: isolate file-scope helpers
enum testing
{
    zero, one, two
};
} using namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum; // libcis


template <>
struct std::is_error_code_enum<testing> : public std::true_type {};

namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum { // libcis: isolate file-scope helpers
std::error_code
make_error_code(testing x)
{
    return std::error_code(static_cast<int>(x), std::generic_category());
}
} using namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum; // libcis


void test_diagnostics_syserr_syserr_errcode_syserr_errcode_constructors_ErrorCodeEnum()
{
    {
        std::error_code ec(two);
        assert(ec.value() == 2);
        assert(ec.category() == std::generic_category());
    }

  return;
}
