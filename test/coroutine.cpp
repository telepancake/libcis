// Tests for <coroutine>
//
// NOTE: Actual coroutine execution (co_await, co_yield, co_return) requires
// -fcoroutines which is NOT part of the libcis build flags. All tests here
// exercise only the *library types* statically:
//   - coroutine_traits (static member check)
//   - coroutine_handle<> address round-trips
//   - suspend_always / suspend_never await_ready semantics
//   - noop_coroutine handle
// No live coroutine frames are instantiated.
#include <coroutine>
#include <type_traits>
#include "test.h"

// ---- coroutine_traits --------------------------------------------------

struct fake_promise { using value_type = int; };
struct fake_coroutine { using promise_type = fake_promise; };
struct no_promise {};

// SFINAE helper: does coroutine_traits<T> have promise_type?
template<class T, class = void>
struct has_coro_promise_type : std::false_type {};
template<class T>
struct has_coro_promise_type<T, std::void_t<typename std::coroutine_traits<T>::promise_type>>
    : std::true_type {};

void test_coroutine_traits() {
    // coroutine_traits with promise_type
    static_assert(__is_same(
        std::coroutine_traits<fake_coroutine>::promise_type,
        fake_promise));

    // coroutine_traits without promise_type has no promise_type member
    static_assert(!has_coro_promise_type<no_promise>::value);
    static_assert(has_coro_promise_type<fake_coroutine>::value);
}

// ---- coroutine_handle<void> address round-trip -------------------------

void test_coroutine_handle_void() {
    // null handle
    std::coroutine_handle<> h;
    CHECK(!h);
    CHECK(h.address() == nullptr);

    // from_address / address round-trip with a dummy pointer
    int dummy = 42;
    void* p = &dummy;
    auto h2 = std::coroutine_handle<>::from_address(p);
    CHECK(h2.address() == p);
    CHECK(static_cast<bool>(h2));

    // nullptr assignment
    h2 = nullptr;
    CHECK(!h2);
    CHECK(h2.address() == nullptr);
}

// ---- coroutine_handle<Promise> address round-trip ----------------------

void test_coroutine_handle_promise() {
    // null handle
    std::coroutine_handle<fake_promise> h;
    CHECK(!h);
    CHECK(h.address() == nullptr);

    // from_address / address round-trip
    int dummy = 0;
    void* p = &dummy;
    auto h2 = std::coroutine_handle<fake_promise>::from_address(p);
    CHECK(h2.address() == p);

    // Implicit conversion to coroutine_handle<>
    std::coroutine_handle<> hbase = h2;
    CHECK(hbase.address() == p);
}

// ---- coroutine_handle comparison / equality ----------------------------

void test_coroutine_handle_compare() {
    int x = 0, y = 0;
    auto h1 = std::coroutine_handle<>::from_address(&x);
    auto h2 = std::coroutine_handle<>::from_address(&x);
    auto h3 = std::coroutine_handle<>::from_address(&y);

    CHECK(h1 == h2);
    CHECK(!(h1 == h3));
}

// ---- suspend_never / suspend_always ------------------------------------

void test_coroutine_trivial_awaitables() {
    std::suspend_never sn;
    CHECK(sn.await_ready() == true);

    std::suspend_always sa;
    CHECK(sa.await_ready() == false);

    // await_suspend / await_resume are no-ops; just call them
    std::coroutine_handle<> null_handle;
    sn.await_suspend(null_handle);
    sn.await_resume();
    sa.await_suspend(null_handle);
    sa.await_resume();
}

// ---- noop_coroutine ----------------------------------------------------

void test_coroutine_noop() {
    auto nh = std::noop_coroutine();

    // Always "not done" and non-null
    CHECK(static_cast<bool>(nh));
    CHECK(!nh.done());
    CHECK(nh.address() != nullptr);

    // Two calls return handles with the same address (single static frame)
    auto nh2 = std::noop_coroutine();
    CHECK(nh.address() == nh2.address());

    // Conversion to coroutine_handle<>
    std::coroutine_handle<> base = nh;
    CHECK(base.address() == nh.address());

    // Operations are no-ops
    nh.resume();
    nh.destroy();
    nh();
}

// ---- static properties -------------------------------------------------

void test_coroutine_static_properties() {
    // suspend_never / suspend_always are trivially constructible
    static_assert(__is_trivially_constructible(std::suspend_never));
    static_assert(__is_trivially_constructible(std::suspend_always));

    // coroutine_handle<> is trivially copyable (pointer wrapper)
    static_assert(__is_trivially_copyable(std::coroutine_handle<>));
}
