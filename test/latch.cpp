// Tests for <latch>.
// Single-threaded sanity checks only.
#include <latch>
#include "test.h"

void test_latch_basic() {
    std::latch l(3);
    CHECK(!l.try_wait());
    l.count_down();
    CHECK(!l.try_wait());
    l.count_down();
    CHECK(!l.try_wait());
    l.count_down();
    CHECK(l.try_wait());
    // wait() on a completed latch should return immediately.
    l.wait();
}

void test_latch_arrive_and_wait() {
    // count_down to 1, then arrive_and_wait counts the last one down and waits.
    std::latch l(1);
    l.arrive_and_wait();
    CHECK(l.try_wait());
}

void test_latch_count_down_by_n() {
    std::latch l(5);
    l.count_down(3);
    CHECK(!l.try_wait());
    l.count_down(2);
    CHECK(l.try_wait());
}

void test_latch_max() {
    static_assert(std::latch::max() > 0);
}
