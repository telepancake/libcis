// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_base_storage_pword).
// main -> test_input_output_iostreams_base_ios_base_ios_base_storage_pword; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// void*& pword(int idx);

// This test compiles but never completes when compiled against the MSVC STL
// UNSUPPORTED: stdlib=msvc

#include <ios>
#include <string>
#include <cassert>
#include <cstdint>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_ios_base_storage_pword { // libcis: isolate file-scope helpers
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};
} using namespace libcis_ns_input_output_iostreams_base_ios_base_ios_base_storage_pword; // libcis


void test_input_output_iostreams_base_ios_base_ios_base_storage_pword()
{
    test t;
    std::ios_base& b = t;
    for (std::intptr_t i = 0; i < 10000; ++i)
    {
        assert(b.pword(i) == 0);
        b.pword(i) = (void*)i;
        assert(b.pword(i) == (void*)i);
        for (std::intptr_t j = 0; j <= i; ++j)
            assert(b.pword(j) == (void*)j);
    }

  return;
}
