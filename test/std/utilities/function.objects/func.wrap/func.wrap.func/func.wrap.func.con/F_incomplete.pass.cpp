// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_incomplete).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_incomplete; file-scope helpers isolated in anon namespace.
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

// template<class F> function(F);

// Allow incomplete argument types in the __is_callable check

#include <functional>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_incomplete { // libcis: isolate file-scope helpers
struct X{
    typedef std::function<void(X&)> callback_type;
    virtual ~X() {}
private:
    callback_type _cb;
};

struct IncompleteReturnType {
  std::function<IncompleteReturnType ()> fn;
};


int called = 0;
IncompleteReturnType test_fn() {
  ++called;
  IncompleteReturnType I;
  return I;
}

// See llvm.org/PR34298
void test_pr34298()
{
  static_assert(std::is_copy_constructible<IncompleteReturnType>::value, "");
  static_assert(std::is_copy_assignable<IncompleteReturnType>::value, "");
  {
    IncompleteReturnType X;
    X.fn = test_fn;
    const IncompleteReturnType& CX = X;
    IncompleteReturnType X2 = CX;
    assert(X2.fn);
    assert(called == 0);
    X2.fn();
    assert(called == 1);
  }
  {
    IncompleteReturnType Empty;
    IncompleteReturnType X2 = Empty;
    assert(!X2.fn);
  }
}
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_incomplete; // libcis


void test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_incomplete() {
  test_pr34298();

  return;
}
