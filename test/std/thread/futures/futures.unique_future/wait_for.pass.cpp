// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_unique_future_wait_for).
// main -> test_thread_futures_futures_unique_future_wait_for; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03

// ALLOW_RETRIES: 3

// <future>

// class future<R>

// template <class Rep, class Period>
//   future_status
//   wait_for(const chrono::duration<Rep, Period>& rel_time) const;

#include <cassert>
#include <chrono>
#include <future>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_futures_futures_unique_future_wait_for { // libcis: isolate file-scope helpers
typedef std::chrono::milliseconds ms;

static const ms sleepTime(500);
static const ms waitTime(5000);

void func1(std::promise<int> p)
{
  std::this_thread::sleep_for(sleepTime);
  p.set_value(3);
}

int j = 0;

void func3(std::promise<int&> p)
{
  std::this_thread::sleep_for(sleepTime);
  j = 5;
  p.set_value(j);
}

void func5(std::promise<void> p)
{
  std::this_thread::sleep_for(sleepTime);
  p.set_value();
}

template <typename T, typename F>
void test(F func, bool waitFirst) {
  typedef std::chrono::high_resolution_clock Clock;
  std::promise<T> p;
  std::future<T> f = p.get_future();
  Clock::time_point t1, t0 = Clock::now();
  support::make_test_thread(func, std::move(p)).detach();
  assert(f.valid());
  assert(f.wait_for(ms(1)) == std::future_status::timeout);
  assert(f.valid());
  if (waitFirst) {
    f.wait();
    assert(f.valid());
    t1 = Clock::now();
    assert(f.wait_for(ms(waitTime)) == std::future_status::ready);
    assert(f.valid());
  } else {
    assert(f.wait_for(ms(waitTime)) == std::future_status::ready);
    assert(f.valid());
    t1 = Clock::now();
    f.wait();
    assert(f.valid());
  }
  assert(t1 - t0 >= sleepTime);
}
} using namespace libcis_ns_thread_futures_futures_unique_future_wait_for; // libcis


void test_thread_futures_futures_unique_future_wait_for()
{
  test<int>(func1, true);
  test<int&>(func3, true);
  test<void>(func5, true);
  test<int>(func1, false);
  test<int&>(func3, false);
  test<void>(func5, false);
  return;
}
