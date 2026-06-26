// transferred+adapted from libc++ by tools/transfer.py (slug=atomics_atomics_ref_notify_one_dca7a220).
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-threads
// XFAIL: availability-synchronization_library-missing
// XFAIL: !has-64-bit-atomics
// XFAIL: !has-1024-bit-atomics

// void notify_one() const noexcept;

#include <atomic>
#include <cassert>
#include <thread>
#include <type_traits>
#include <vector>

#include "atomic_helpers.h"
#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_atomics_atomics_ref_notify_one_dca7a220 { // libcis
template <typename T>
struct TestNotifyOne {
  void operator()() const {
    T x(T(1));
    std::atomic_ref<T> const a(x);

    std::thread t = support::make_test_thread([&]() {
      a.store(T(3));
      a.notify_one();
    });
    a.wait(T(1));
    assert(a.load() == T(3));
    t.join();
    ASSERT_NOEXCEPT(a.notify_one());
  }
};

int main(int, char**) {
  TestEachAtomicType<TestNotifyOne>()();
  return 0;

    return 0;
}
} // libcis_ns_atomics_atomics_ref_notify_one_dca7a220 (libcis)

