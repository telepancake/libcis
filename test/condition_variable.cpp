// test/condition_variable.cpp — runtime tests for <condition_variable>.
// Exercises: cv_status, condition_variable (wait, wait_for, wait_until, notify),
//   condition_variable_any.
// Uses raw pthreads for a multi-threaded ping-pong test; also single-threaded
// timeout tests.
#include "test.h"
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <pthread.h>

// ---- cv_status values ----
void test_condition_variable_cv_status() {
    static_assert(std::cv_status::no_timeout != std::cv_status::timeout);
}

// ---- Single-threaded: wait_for that immediately times out ----
void test_condition_variable_wait_for_timeout() {
    std::mutex m;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lk(m);

    // wait_for with zero duration should timeout immediately
    auto st = cv.wait_for(lk, std::chrono::milliseconds(0));
    CHECK(st == std::cv_status::timeout);

    // wait_for with negative duration should timeout immediately
    st = cv.wait_for(lk, std::chrono::milliseconds(-1));
    CHECK(st == std::cv_status::timeout);

    // wait_for with short duration — no notifier, should timeout
    st = cv.wait_for(lk, std::chrono::milliseconds(5));
    CHECK(st == std::cv_status::timeout);
}

// ---- Single-threaded: wait_until already past ----
void test_condition_variable_wait_until_past() {
    std::mutex m;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lk(m);

    auto past = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    auto st = cv.wait_until(lk, past);
    CHECK(st == std::cv_status::timeout);
}

// ---- wait with predicate — predicate already true ----
void test_condition_variable_wait_pred_true() {
    std::mutex m;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lk(m);
    bool ready = true;
    cv.wait(lk, [&]{ return ready; });  // should return immediately
    CHECK(true); // reached here
}

// ---- wait_for with predicate that times out ----
void test_condition_variable_wait_for_pred_timeout() {
    std::mutex m;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lk(m);
    bool flag = false;
    bool result = cv.wait_for(lk, std::chrono::milliseconds(5), [&]{ return flag; });
    CHECK(!result);  // timed out, pred still false
}

// ---- Ping-pong between two pthreads ----
// Thread A sets "ping", notifies, waits for "pong".
// Thread B waits for "ping", sets "pong", notifies.
// Single exchange.

struct PingPong {
    std::mutex m;
    std::condition_variable cv;
    int state = 0; // 0: idle, 1: ping, 2: pong
};

static void* pong_thread(void* arg) {
    PingPong* pp = static_cast<PingPong*>(arg);
    std::unique_lock<std::mutex> lk(pp->m);
    pp->cv.wait(lk, [pp]{ return pp->state == 1; }); // wait for ping
    pp->state = 2; // pong
    pp->cv.notify_one();
    return nullptr;
}

void test_condition_variable_ping_pong() {
    PingPong pp;
    pthread_t t;
    pthread_create(&t, nullptr, pong_thread, &pp);

    {
        std::unique_lock<std::mutex> lk(pp.m);
        pp.state = 1;  // ping
        pp.cv.notify_one();
        pp.cv.wait(lk, [&pp]{ return pp.state == 2; });
    }
    CHECK(pp.state == 2);
    pthread_join(t, nullptr);
}

// ---- condition_variable_any ----
void test_condition_variable_any_basic() {
    std::mutex m;
    std::condition_variable_any cva;
    std::unique_lock<std::mutex> lk(m);

    // wait_for should timeout since no notifier
    auto st = cva.wait_for(lk, std::chrono::milliseconds(5));
    CHECK(st == std::cv_status::timeout);
}

void test_condition_variable_any_pred() {
    std::mutex m;
    std::condition_variable_any cva;
    std::unique_lock<std::mutex> lk(m);
    bool flag = true;
    cva.wait(lk, [&]{ return flag; }); // pred true immediately
    CHECK(true);
}

// ---- Ping-pong with condition_variable_any ----
struct PingPongAny {
    std::mutex m;
    std::condition_variable_any cva;
    int state = 0;
};

static void* pong_thread_any(void* arg) {
    PingPongAny* pp = static_cast<PingPongAny*>(arg);
    std::unique_lock<std::mutex> lk(pp->m);
    pp->cva.wait(lk, [pp]{ return pp->state == 1; });
    pp->state = 2;
    pp->cva.notify_one();
    return nullptr;
}

void test_condition_variable_any_ping_pong() {
    PingPongAny pp;
    pthread_t t;
    pthread_create(&t, nullptr, pong_thread_any, &pp);

    {
        std::unique_lock<std::mutex> lk(pp.m);
        pp.state = 1;
        pp.cva.notify_one();
        pp.cva.wait(lk, [&pp]{ return pp.state == 2; });
    }
    CHECK(pp.state == 2);
    pthread_join(t, nullptr);
}
