// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_members_static_sync_with_stdio_a52b9e56).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// bool sync_with_stdio(bool sync = true);

#include <ios>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_ios_members_static_sync_with_stdio_a52b9e56 { // libcis
int main(int, char**)
{
    assert( std::ios_base::sync_with_stdio(false));
    assert(!std::ios_base::sync_with_stdio(false));
    assert(!std::ios_base::sync_with_stdio(true));
    assert( std::ios_base::sync_with_stdio(true));
    assert( std::ios_base::sync_with_stdio());
    assert( std::ios_base::sync_with_stdio(false));
    assert(!std::ios_base::sync_with_stdio());
    assert( std::ios_base::sync_with_stdio());

  return 0;

    return 0;
}
} // libcis_ns_input_output_iostreams_base_ios_base_ios_members_static_sync_with_stdio_a52b9e56 (libcis)

