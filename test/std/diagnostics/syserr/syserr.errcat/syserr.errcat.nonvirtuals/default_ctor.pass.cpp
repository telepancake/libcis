// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcat_syserr_errcat_nonvirtuals_default_ctor_c5aa7430).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11
// <system_error>

// class error_category

// constexpr error_category() noexcept;

#include <system_error>
#include <type_traits>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcat_syserr_errcat_nonvirtuals_default_ctor_c5aa7430 { // libcis
class test1
    : public std::error_category
{
public:
    constexpr test1() = default;  // won't compile if error_category() is not constexpr
    virtual const char* name() const noexcept {return nullptr;}
    virtual std::string message(int) const {return std::string();}
};

int main(int, char**)
{
    static_assert(std::is_nothrow_default_constructible<test1>::value,
                                 "error_category() must exist and be noexcept");

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcat_syserr_errcat_nonvirtuals_default_ctor_c5aa7430 (libcis)

