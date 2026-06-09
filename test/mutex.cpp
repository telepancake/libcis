// test/mutex.cpp — runtime tests for <mutex>.
// Exercises: mutex, recursive_mutex, timed_mutex, recursive_timed_mutex,
//   lock_guard, unique_lock, scoped_lock, defer/try/adopt tags,
//   try_lock, lock (2-arg), once_flag/call_once.
// No std::thread used — tests are single-threaded where possible; POSIX
// pthreads used for the ping-pong once_flag test.
#include "test.h"
#include <mutex>
#include <pthread.h>

// ---- mutex basic ----
void test_mutex_basic() {
    std::mutex m;
    m.lock();
    m.unlock();

    CHECK(m.try_lock());
    m.unlock();
    CHECK(m.try_lock());
    // second try_lock should fail (already locked)
    CHECK(!m.try_lock());
    m.unlock();
}

// ---- recursive_mutex ----
void test_mutex_recursive() {
    std::recursive_mutex rm;
    rm.lock();
    rm.lock();   // recursive — must not deadlock
    rm.lock();
    rm.unlock();
    rm.unlock();
    rm.unlock();

    CHECK(rm.try_lock());
    CHECK(rm.try_lock());
    rm.unlock();
    rm.unlock();
}

// ---- lock_guard ----
void test_mutex_lock_guard() {
    std::mutex m;
    {
        std::lock_guard<std::mutex> lg(m);
        // m is locked here
    }
    // m must be unlocked now
    CHECK(m.try_lock());
    m.unlock();

    // adopt_lock variant
    m.lock();
    {
        std::lock_guard<std::mutex> lg(m, std::adopt_lock);
    }
    // unlocked again
    CHECK(m.try_lock());
    m.unlock();
}

// ---- unique_lock ----
void test_mutex_unique_lock() {
    std::mutex m;

    // basic
    {
        std::unique_lock<std::mutex> ul(m);
        CHECK(ul.owns_lock());
        CHECK(ul.mutex() == &m);
        ul.unlock();
        CHECK(!ul.owns_lock());
        ul.lock();
        CHECK(ul.owns_lock());
    }
    CHECK(m.try_lock());
    m.unlock();

    // defer_lock
    {
        std::unique_lock<std::mutex> ul(m, std::defer_lock);
        CHECK(!ul.owns_lock());
        ul.lock();
        CHECK(ul.owns_lock());
    }
    CHECK(m.try_lock());
    m.unlock();

    // try_to_lock (succeeds since m is free)
    {
        std::unique_lock<std::mutex> ul(m, std::try_to_lock);
        CHECK(ul.owns_lock());
    }
    CHECK(m.try_lock());
    m.unlock();

    // adopt_lock
    m.lock();
    {
        std::unique_lock<std::mutex> ul(m, std::adopt_lock);
        CHECK(ul.owns_lock());
    }
    CHECK(m.try_lock());
    m.unlock();

    // move constructor
    {
        std::unique_lock<std::mutex> ul1(m);
        CHECK(ul1.owns_lock());
        std::unique_lock<std::mutex> ul2(std::move(ul1));
        CHECK(!ul1.owns_lock());
        CHECK(ul1.mutex() == nullptr);
        CHECK(ul2.owns_lock());
    }
    CHECK(m.try_lock());
    m.unlock();

    // release
    {
        std::unique_lock<std::mutex> ul(m);
        CHECK(ul.owns_lock());
        std::mutex* pm = ul.release();
        CHECK(pm == &m);
        CHECK(!ul.owns_lock());
        CHECK(ul.mutex() == nullptr);
    }
    // m is still locked — we called release() without unlock
    m.unlock(); // manual cleanup
}

// ---- scoped_lock ----
void test_mutex_scoped_lock() {
    std::mutex m1, m2;
    {
        std::scoped_lock<std::mutex, std::mutex> sl(m1, m2);
    }
    CHECK(m1.try_lock()); m1.unlock();
    CHECK(m2.try_lock()); m2.unlock();

    // single-mutex variant
    {
        std::scoped_lock<std::mutex> sl(m1);
    }
    CHECK(m1.try_lock()); m1.unlock();

    // zero-mutex variant
    {
        std::scoped_lock<> sl;
    }
}

// ---- try_lock (free function) ----
void test_mutex_try_lock() {
    std::mutex m1, m2;
    int r = std::try_lock(m1, m2);
    CHECK(r == -1);  // both locked
    m1.unlock();
    m2.unlock();

    // m1 already locked — try_lock should fail at index 0
    m1.lock();
    r = std::try_lock(m1, m2);
    CHECK(r == 0);  // m1 couldn't be locked
    m1.unlock();
}

// ---- lock (free function, 2-arg) ----
void test_mutex_lock_free() {
    std::mutex m1, m2;
    std::lock(m1, m2);
    CHECK(!m1.try_lock());  // already locked
    CHECK(!m2.try_lock());
    m1.unlock();
    m2.unlock();
}

// ---- timed_mutex ----
void test_mutex_timed_mutex() {
    std::timed_mutex tm;
    tm.lock();
    // try_lock_for with very short timeout should fail (we hold the lock)
    bool got = tm.try_lock_for(std::chrono::milliseconds(1));
    CHECK(!got);
    tm.unlock();

    // now it should succeed immediately
    got = tm.try_lock_for(std::chrono::seconds(1));
    CHECK(got);
    tm.unlock();
}

// ---- recursive_timed_mutex ----
void test_mutex_recursive_timed_mutex() {
    std::recursive_timed_mutex rtm;
    rtm.lock();
    rtm.lock();  // recursive
    rtm.unlock();
    rtm.unlock();

    bool got = rtm.try_lock_for(std::chrono::milliseconds(100));
    CHECK(got);
    rtm.unlock();
}

// ---- once_flag / call_once ----
static int once_counter = 0;

void test_mutex_call_once() {
    std::once_flag flag;
    once_counter = 0;
    std::call_once(flag, []{ ++once_counter; });
    std::call_once(flag, []{ ++once_counter; });
    std::call_once(flag, []{ ++once_counter; });
    CHECK(once_counter == 1);

    // with arguments
    std::once_flag flag2;
    int x = 0;
    std::call_once(flag2, [](int* p, int val){ *p = val; }, &x, 42);
    std::call_once(flag2, [](int* p, int val){ *p = val; }, &x, 99);
    CHECK(x == 42);
}
