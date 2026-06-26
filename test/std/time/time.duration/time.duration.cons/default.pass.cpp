// transferred+adapted from libc++ by tools/transfer.py (slug=time_time_duration_time_duration_cons_default_3275c781).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// duration

// duration() = default;

// Rep must be default initialized, not initialized with 0

#include <chrono>
#include <cassert>

#include "test_macros.h"
#include "../../rep.h"

namespace libcis_ns_time_time_duration_time_duration_cons_default_3275c781 { // libcis
template <class D>
void
test()
{
    D d;
    assert(d.count() == typename D::rep());
#if TEST_STD_VER >= 11
    constexpr D d2 = D();
    static_assert(d2.count() == typename D::rep(), "");
#endif
}

int main(int, char**)
{
    test<std::chrono::duration<Rep> >();

  return 0;

    return 0;
}
} // libcis_ns_time_time_duration_time_duration_cons_default_3275c781 (libcis)

