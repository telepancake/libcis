// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// function& operator=(nullptr_t);

#include <functional>
#include <cassert>

#include "count_new.h"

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign { // libcis: isolate file-scope helpers
class A
{
    int data_[10];
public:
    static int count;

    A()
    {
        ++count;
        for (int i = 0; i < 10; ++i)
            data_[i] = i;
    }

    A(const A&) {++count;}

    ~A() {--count;}

    int operator()(int i) const
    {
        for (int j = 0; j < 10; ++j)
            i += data_[j];
        return i;
    }
};
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign; // libcis


int A::count = 0;

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign { // libcis: isolate file-scope helpers
int g(int) {return 0;}
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign; // libcis


void test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign()
{
    globalMemCounter.reset();
    assert(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = A();
    assert(A::count == 1);
    assert(globalMemCounter.checkOutstandingNewLessThanOrEqual(1));
    RTTI_ASSERT(f.target<A>());
    f = nullptr;
    assert(A::count == 0);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<A>() == 0);
    }
    {
    std::function<int(int)> f = g;
    assert(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>());
    RTTI_ASSERT(f.target<A>() == 0);
    f = nullptr;
    assert(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    }

  return;
}
