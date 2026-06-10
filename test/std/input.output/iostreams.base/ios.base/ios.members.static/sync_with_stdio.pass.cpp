// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_members_static_sync_with_stdio).
// main -> test_input_output_iostreams_base_ios_base_ios_members_static_sync_with_stdio; file-scope helpers isolated in anon namespace.
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

void test_input_output_iostreams_base_ios_base_ios_members_static_sync_with_stdio()
{
    assert( std::ios_base::sync_with_stdio(false));
    assert(!std::ios_base::sync_with_stdio(false));
    assert(!std::ios_base::sync_with_stdio(true));
    assert( std::ios_base::sync_with_stdio(true));
    assert( std::ios_base::sync_with_stdio());
    assert( std::ios_base::sync_with_stdio(false));
    assert(!std::ios_base::sync_with_stdio());
    assert( std::ios_base::sync_with_stdio());

  return;
}
