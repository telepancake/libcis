// Tests for <exception>, ported from libc++ test suite.
// Covers: exception, bad_exception, terminate_handler, set_terminate,
//         get_terminate, terminate (declared; called only via set/get).
// Each test_* function exercises one aspect. No main(); auto-generated.
#include <exception>
#include "test.h"

//===----------------------------------------------------------------------===//
// test_exception_class
// std::exception is a non-trivial class with a virtual destructor and what().
//===----------------------------------------------------------------------===//
void test_exception_class() {
    // exception is a class
    static_assert(__is_class(std::exception));
    // exception has a virtual destructor
    static_assert(__has_virtual_destructor(std::exception));
    // exception is polymorphic
    static_assert(__is_polymorphic(std::exception));

    // Default construction and what()
    std::exception e;
    // what() must return a non-null pointer (string content is implementation-defined)
    CHECK(e.what() != nullptr);

    // Copy construction
    std::exception e2 = e;
    CHECK(e2.what() != nullptr);

    // Copy assignment
    std::exception e3;
    e3 = e;
    CHECK(e3.what() != nullptr);
}

//===----------------------------------------------------------------------===//
// test_exception_bad_exception
// std::bad_exception derives from std::exception.
//===----------------------------------------------------------------------===//
void test_exception_bad_exception() {
    static_assert(__is_base_of(std::exception, std::bad_exception));
    static_assert(__is_class(std::bad_exception));
    static_assert(__has_virtual_destructor(std::bad_exception));

    std::bad_exception be;
    CHECK(be.what() != nullptr);

    // Accessible through base pointer
    std::exception* p = &be;
    CHECK(p->what() != nullptr);

    // Copy
    std::bad_exception be2 = be;
    CHECK(be2.what() != nullptr);
}

//===----------------------------------------------------------------------===//
// test_exception_terminate_handler_type
// terminate_handler is a function pointer type.
//===----------------------------------------------------------------------===//
void test_exception_terminate_handler_type() {
    // terminate_handler is void(*)()
    static_assert(__is_same(std::terminate_handler, void(*)()));

    // Can be null-initialised
    std::terminate_handler h = nullptr;
    CHECK(h == nullptr);

    // A plain function matches the type
    struct Local {
        static void handler() {}
    };
    std::terminate_handler h2 = Local::handler;
    CHECK(h2 != nullptr);
}

//===----------------------------------------------------------------------===//
// test_exception_set_get_terminate
// set_terminate returns the previous handler; get_terminate returns current.
//===----------------------------------------------------------------------===//
void test_exception_set_get_terminate() {
    struct Handlers {
        static void handler_a() {}
        static void handler_b() {}
    };

    // Save the original handler so we can restore it.
    std::terminate_handler orig = std::get_terminate();

    // Install handler_a
    std::terminate_handler prev = std::set_terminate(Handlers::handler_a);
    // prev should equal the original
    CHECK(prev == orig);
    CHECK(std::get_terminate() == Handlers::handler_a);

    // Replace with handler_b
    std::terminate_handler prev2 = std::set_terminate(Handlers::handler_b);
    CHECK(prev2 == Handlers::handler_a);
    CHECK(std::get_terminate() == Handlers::handler_b);

    // Restore original
    std::set_terminate(orig);
    CHECK(std::get_terminate() == orig);
}
