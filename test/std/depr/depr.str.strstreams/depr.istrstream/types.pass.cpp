// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_istrstream_types_408e6e1f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class istrstream
//     : public basic_istream<char>
// {
//     ...

#include <strstream>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_istrstream_types_408e6e1f { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::istream, std::istrstream>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_istrstream_types_408e6e1f (libcis)

