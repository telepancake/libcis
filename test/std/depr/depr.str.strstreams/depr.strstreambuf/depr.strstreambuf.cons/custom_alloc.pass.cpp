// transferred+adapted from libc++ by tools/transfer.py (slug=depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_cons_custom_alloc_34d3bebb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS -D_LIBCPP_ENABLE_CXX26_REMOVED_STRSTREAM

// <strstream>

// class strstreambuf

// strstreambuf(void* (*palloc_arg)(size_t), void (*pfree_arg)(void*));

#include <strstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_cons_custom_alloc_34d3bebb { // libcis
int called = 0;

void* my_alloc(std::size_t)
{
    static char buf[10000];
    ++called;
    return buf;
}

void my_free(void*)
{
    ++called;
}

struct test
    : std::strstreambuf
{
    test(void* (*palloc_arg)(std::size_t), void (*pfree_arg)(void*))
        : std::strstreambuf(palloc_arg, pfree_arg) {}
    virtual int_type overflow(int_type c)
        {return std::strstreambuf::overflow(c);}
};

int main(int, char**)
{
    {
        test s(my_alloc, my_free);
        assert(called == 0);
        s.overflow('a');
        assert(called == 1);
    }
    assert(called == 2);

  return 0;

    return 0;
}
} // libcis_ns_depr_depr_str_strstreams_depr_strstreambuf_depr_strstreambuf_cons_custom_alloc_34d3bebb (libcis)

