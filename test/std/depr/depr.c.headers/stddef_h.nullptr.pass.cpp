// AST-transferred from libc++ by tools/transfer.py (slug=depr_depr_c_headers_stddef_h_nullptr).
// main -> test_depr_depr_c_headers_stddef_h_nullptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stddef.h>

#include <stddef.h>
#include <cassert>

void test_depr_depr_c_headers_stddef_h_nullptr() {
    {
        void *p = NULL;
        assert(!p);
    }
    {
        void *p = nullptr;
        assert(!p);
    }

    return;
}
