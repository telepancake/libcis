// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_exception_except_nested_ctor_default).
// main -> test_language_support_support_exception_except_nested_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <exception>

// class nested_exception;

// nested_exception() throw();

#include <exception>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_exception_except_nested_ctor_default { // libcis: isolate file-scope helpers
class A
{
    int data_;
public:
    explicit A(int data) : data_(data) {}

    friend bool operator==(const A& x, const A& y) {return x.data_ == y.data_;}
};
} using namespace libcis_ns_language_support_support_exception_except_nested_ctor_default; // libcis


void test_language_support_support_exception_except_nested_ctor_default()
{
    {
        std::nested_exception e;
        assert(e.nested_ptr() == nullptr);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        try
        {
            throw A(2);
            assert(false);
        }
        catch (const A&)
        {
            std::nested_exception e;
            assert(e.nested_ptr() != nullptr);
            try
            {
                std::rethrow_exception(e.nested_ptr());
                assert(false);
            }
            catch (const A& a)
            {
                assert(a == A(2));
            }
        }
    }
#endif

  return;
}
