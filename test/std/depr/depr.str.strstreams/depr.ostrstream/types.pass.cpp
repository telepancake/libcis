// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_ostrstream_types_7e7eb76e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class ostrstream
//     : public basic_ostream<char>
// {
//     ...

#include <strstream>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_ostrstream_types_7e7eb76e { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::ostream, std::ostrstream>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_ostrstream_types_7e7eb76e (libcis)

