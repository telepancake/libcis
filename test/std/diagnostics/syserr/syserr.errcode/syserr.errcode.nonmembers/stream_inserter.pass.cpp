// transferred+adapted from libc++ by tools/transfer.py (slug=diagnostics_syserr_syserr_errcode_syserr_errcode_nonmembers_stream_inserter_6cc42df8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-localization

// <system_error>

// class error_code

// template <class charT, class traits>
//   basic_ostream<charT,traits>&
//   operator<<(basic_ostream<charT,traits>& os, const error_code& ec);

#include <system_error>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_nonmembers_stream_inserter_6cc42df8 { // libcis
int main(int, char**)
{
    std::ostringstream out;
    out << std::error_code(std::io_errc::stream);
    assert(out.str() == "iostream:1");

  return 0;

    return 0;
}
} // libcis_ns_diagnostics_syserr_syserr_errcode_syserr_errcode_nonmembers_stream_inserter_6cc42df8 (libcis)

