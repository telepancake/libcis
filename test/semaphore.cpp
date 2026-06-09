// Tests for <semaphore> (counting_semaphore, binary_semaphore).
// Single-threaded sanity checks only.
#include <semaphore>
#include <chrono>
#include "test.h"

void test_semaphore_counting_basic() {
    // Start with count 3, acquire three times.
    std::counting_semaphore<5> s(3);
    CHECK(s.try_acquire());
    CHECK(s.try_acquire());
    CHECK(s.try_acquire());
    // Count is now 0, next try_acquire should fail.
    CHECK(!s.try_acquire());
    // Release two and verify we can acquire again.
    s.release(2);
    CHECK(s.try_acquire());
    CHECK(s.try_acquire());
    CHECK(!s.try_acquire());
}

void test_semaphore_acquire_after_release() {
    // Start at 0, release then acquire.
    std::counting_semaphore<10> s(0);
    CHECK(!s.try_acquire());
    s.release();
    CHECK(s.try_acquire());
    CHECK(!s.try_acquire());
}

void test_semaphore_binary() {
    std::binary_semaphore b(1);
    CHECK(b.try_acquire());   // acquires the single token
    CHECK(!b.try_acquire());  // no token left
    b.release();
    CHECK(b.try_acquire());   // token back
}

void test_semaphore_try_acquire_for_zero() {
    std::counting_semaphore<4> s(0);
    using ns = std::chrono::nanoseconds;
    // Zero timeout on empty semaphore must return false immediately.
    CHECK(!s.try_acquire_for(ns(0)));
    s.release();
    // Non-zero timeout on available semaphore must succeed.
    CHECK(s.try_acquire_for(std::chrono::milliseconds(1)));
}

void test_semaphore_try_acquire_until_past() {
    std::counting_semaphore<4> s(0);
    auto past = std::chrono::steady_clock::now() - std::chrono::milliseconds(1);
    // Past deadline on empty semaphore must return false.
    CHECK(!s.try_acquire_until(past));
    s.release();
    // Past deadline but token available -- still a try_acquire.
    CHECK(s.try_acquire_until(past));
}

void test_semaphore_max() {
    static_assert(std::counting_semaphore<7>::max() == 7);
    static_assert(std::binary_semaphore::max() == 1);
}
