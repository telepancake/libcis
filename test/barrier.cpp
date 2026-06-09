// Tests for <barrier>.
// Single-threaded sanity checks only.
#include <barrier>
#include "test.h"

void test_barrier_arrive_and_wait_single() {
    // A barrier with count=1: one arrive() completes the phase.
    std::barrier<> b(1);
    b.arrive_and_wait();
    // We get here: phase completed successfully.
    b.arrive_and_wait(); // second phase also completes
}

void test_barrier_completion_called() {
    int calls = 0;
    std::barrier b(1, [&calls]() noexcept { ++calls; });
    b.arrive_and_wait();
    CHECK(calls == 1);
    b.arrive_and_wait();
    CHECK(calls == 2);
}

void test_barrier_arrive_then_wait() {
    std::barrier<> b(1);
    auto tok = b.arrive();
    b.wait(static_cast<std::barrier<>::arrival_token&&>(tok));
}

void test_barrier_arrive_and_drop() {
    // Start with 2 participants; one drops.  After drop the expected count
    // for subsequent phases is 1, so a single arrive_and_wait completes.
    std::barrier<> b(2);
    // First phase: one normal arrive, one arrive_and_drop.
    auto tok = b.arrive();
    b.arrive_and_drop();
    b.wait(static_cast<std::barrier<>::arrival_token&&>(tok));
    // Second phase: only 1 participant remains.
    b.arrive_and_wait();
}

void test_barrier_max() {
    static_assert(std::barrier<>::max() > 0);
}
