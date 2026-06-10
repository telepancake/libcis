// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_std_ios_manip_error_reporting_make_error_code).
// main -> test_input_output_iostreams_base_std_ios_manip_error_reporting_make_error_code; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// error_code make_error_code(io_errc e);

#include <ios>
#include <cassert>

#include "test_macros.h"

void test_input_output_iostreams_base_std_ios_manip_error_reporting_make_error_code()
{
    {
        std::error_code ec = make_error_code(std::io_errc::stream);
        assert(ec.value() == static_cast<int>(std::io_errc::stream));
        assert(ec.category() == std::iostream_category());
    }

  return;
}
