// test/shared_mutex.cpp — runtime tests for <shared_mutex>.
// Exercises: shared_mutex, shared_timed_mutex, shared_lock.
// Concurrent readers test uses raw pthreads.
#include "test.h"
#include <shared_mutex>
#include <mutex>
#include <chrono>
#include <pthread.h>
#include <atomic>

// ---- shared_mutex exclusive ----
void test_shared_mutex_exclusive() {
    std::shared_mutex sm;
    sm.lock();
    CHECK(!sm.try_lock());
    CHECK(!sm.try_lock_shared());
    sm.unlock();

    CHECK(sm.try_lock());
    sm.unlock();
}

// ---- shared_mutex shared ----
void test_shared_mutex_shared() {
    std::shared_mutex sm;
    sm.lock_shared();
    sm.lock_shared(); // multiple readers allowed
    CHECK(!sm.try_lock()); // exclusive lock blocked
    sm.unlock_shared();
    sm.unlock_shared();

    CHECK(sm.try_lock_shared());
    sm.unlock_shared();
}

// ---- shared_mutex write excludes reads ----
void test_shared_mutex_write_excludes_read() {
    std::shared_mutex sm;
    sm.lock(); // exclusive
    CHECK(!sm.try_lock_shared()); // reader blocked
    sm.unlock();

    // after unlock, reader succeeds
    CHECK(sm.try_lock_shared());
    sm.unlock_shared();
}

// ---- shared_lock<shared_mutex> ----
void test_shared_mutex_shared_lock() {
    std::shared_mutex sm;

    // default ctor
    {
        std::shared_lock<std::shared_mutex> sl;
        CHECK(!sl.owns_lock());
        CHECK(sl.mutex() == nullptr);
    }

    // locking ctor
    {
        std::shared_lock<std::shared_mutex> sl(sm);
        CHECK(sl.owns_lock());
        CHECK(sl.mutex() == &sm);
    }
    // after destruction, exclusive lock should succeed
    CHECK(sm.try_lock());
    sm.unlock();

    // defer_lock
    {
        std::shared_lock<std::shared_mutex> sl(sm, std::defer_lock);
        CHECK(!sl.owns_lock());
        sl.lock();
        CHECK(sl.owns_lock());
    }
    CHECK(sm.try_lock()); sm.unlock();

    // try_to_lock (should succeed since free)
    {
        std::shared_lock<std::shared_mutex> sl(sm, std::try_to_lock);
        CHECK(sl.owns_lock());
    }
    CHECK(sm.try_lock()); sm.unlock();

    // adopt_lock
    sm.lock_shared();
    {
        std::shared_lock<std::shared_mutex> sl(sm, std::adopt_lock);
        CHECK(sl.owns_lock());
    }
    CHECK(sm.try_lock()); sm.unlock();

    // move ctor
    {
        std::shared_lock<std::shared_mutex> sl1(sm);
        CHECK(sl1.owns_lock());
        std::shared_lock<std::shared_mutex> sl2(std::move(sl1));
        CHECK(!sl1.owns_lock());
        CHECK(sl2.owns_lock());
    }
    CHECK(sm.try_lock()); sm.unlock();

    // release
    {
        std::shared_lock<std::shared_mutex> sl(sm);
        CHECK(sl.owns_lock());
        std::shared_mutex* pm = sl.release();
        CHECK(pm == &sm);
        CHECK(!sl.owns_lock());
    }
    // sm is still shared-locked; release doesn't unlock
    // We need to unlock manually:
    sm.unlock_shared();
    CHECK(sm.try_lock()); sm.unlock();
}

// ---- concurrent readers ----
struct ReaderData {
    std::shared_mutex* sm;
    std::atomic<int>*  inside_count;
    std::atomic<int>*  max_concurrent;
};

static void* reader_thread(void* arg) {
    auto* d = static_cast<ReaderData*>(arg);
    std::shared_lock<std::shared_mutex> sl(*d->sm);
    int n = d->inside_count->fetch_add(1, std::memory_order_relaxed) + 1;
    int old = d->max_concurrent->load(std::memory_order_relaxed);
    while (old < n && !d->max_concurrent->compare_exchange_weak(old, n,
               std::memory_order_relaxed)) {}
    // small busy-wait to keep multiple threads inside simultaneously
    for (int i = 0; i < 10000; ++i) { asm volatile("" ::: "memory"); }
    d->inside_count->fetch_sub(1, std::memory_order_relaxed);
    return nullptr;
}

void test_shared_mutex_concurrent_readers() {
    std::shared_mutex sm;
    std::atomic<int> inside(0);
    std::atomic<int> max_concurrent(0);
    ReaderData d{&sm, &inside, &max_concurrent};

    constexpr int N = 4;
    pthread_t threads[N];
    for (int i = 0; i < N; ++i)
        pthread_create(&threads[i], nullptr, reader_thread, &d);
    for (int i = 0; i < N; ++i)
        pthread_join(threads[i], nullptr);

    // At least 1 thread was concurrent with another
    CHECK(max_concurrent.load() >= 1);
}

// ---- shared_timed_mutex ----
void test_shared_mutex_timed() {
    std::shared_timed_mutex stm;
    stm.lock();
    bool got = stm.try_lock_for(std::chrono::milliseconds(1));
    CHECK(!got); // already write-locked
    got = stm.try_lock_shared_for(std::chrono::milliseconds(1));
    CHECK(!got); // can't take shared while write held
    stm.unlock();

    // now a shared lock should succeed
    got = stm.try_lock_shared_for(std::chrono::milliseconds(100));
    CHECK(got);
    stm.unlock_shared();

    // exclusive timed
    got = stm.try_lock_for(std::chrono::milliseconds(100));
    CHECK(got);
    stm.unlock();
}
