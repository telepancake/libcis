// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_base_ios_base_storage_iword).
// main -> test_input_output_iostreams_base_ios_base_ios_base_storage_iword; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// class ios_base

// long& iword(int idx);

// This test compiles but never completes when compiled against the MSVC STL
// UNSUPPORTED: stdlib=msvc

#include <ios>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_base_ios_base_storage_iword { // libcis: isolate file-scope helpers
class test
    : public std::ios
{
public:
    test()
    {
        init(0);
    }
};
} using namespace libcis_ns_input_output_iostreams_base_ios_base_ios_base_storage_iword; // libcis


void test_input_output_iostreams_base_ios_base_ios_base_storage_iword()
{
    test t;
    std::ios_base& b = t;
    for (int i = 0; i < 10000; ++i)
    {
        assert(b.iword(i) == 0);
        b.iword(i) = i;
        assert(b.iword(i) == i);
        for (int j = 0; j <= i; ++j)
            assert(b.iword(j) == j);
    }

  return;
}
