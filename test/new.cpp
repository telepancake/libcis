// Tests for <new>, ported from libc++ test suite.
// Covers: align_val_t, nothrow_t/nothrow, bad_alloc, bad_array_new_length,
//         new_handler/set_new_handler/get_new_handler, launder,
//         destroying_delete_t/destroying_delete,
//         hardware_destructive/constructive_interference_size,
//         global operator new/delete, placement new/delete.
// Each test_* function exercises one aspect. No main(); auto-generated.
#include <new>
#include "test.h"

//===----------------------------------------------------------------------===//
// test_new_align_val_t
//===----------------------------------------------------------------------===//
void test_new_align_val_t() {
    // align_val_t is a scoped enum over size_t
    static_assert(__is_enum(std::align_val_t));
    static_assert(__is_same(__underlying_type(std::align_val_t), std::size_t));

    std::align_val_t a = std::align_val_t(16);
    CHECK(static_cast<std::size_t>(a) == 16);
}

//===----------------------------------------------------------------------===//
// test_new_nothrow_t
//===----------------------------------------------------------------------===//
void test_new_nothrow_t() {
    static_assert(__is_class(std::nothrow_t));
    // nothrow is a const nothrow_t object
    static_assert(__is_same(decltype(std::nothrow), const std::nothrow_t));
    // The address is stable across uses (it's an extern object)
    const std::nothrow_t* p = &std::nothrow;
    CHECK(p != nullptr);
}

//===----------------------------------------------------------------------===//
// test_new_bad_alloc
//===----------------------------------------------------------------------===//
void test_new_bad_alloc() {
    static_assert(__is_base_of(std::exception, std::bad_alloc));
    static_assert(__has_virtual_destructor(std::bad_alloc));
    static_assert(__is_polymorphic(std::bad_alloc));

    std::bad_alloc ba;
    CHECK(ba.what() != nullptr);

    // Via base pointer
    std::exception* ep = &ba;
    CHECK(ep->what() != nullptr);
}

//===----------------------------------------------------------------------===//
// test_new_bad_array_new_length
//===----------------------------------------------------------------------===//
void test_new_bad_array_new_length() {
    static_assert(__is_base_of(std::bad_alloc, std::bad_array_new_length));
    static_assert(__is_base_of(std::exception, std::bad_array_new_length));
    static_assert(__has_virtual_destructor(std::bad_array_new_length));

    std::bad_array_new_length banl;
    CHECK(banl.what() != nullptr);

    // Via bad_alloc pointer
    std::bad_alloc* bap = &banl;
    CHECK(bap->what() != nullptr);
}

//===----------------------------------------------------------------------===//
// test_new_handler
//===----------------------------------------------------------------------===//
void test_new_handler() {
    static_assert(__is_same(std::new_handler, void(*)()));

    struct Local {
        static void my_handler() {}
    };

    // Save original
    std::new_handler orig = std::get_new_handler();

    std::new_handler prev = std::set_new_handler(Local::my_handler);
    CHECK(prev == orig);
    CHECK(std::get_new_handler() == Local::my_handler);

    // Restore
    std::set_new_handler(orig);
    CHECK(std::get_new_handler() == orig);
}

//===----------------------------------------------------------------------===//
// test_new_launder
//===----------------------------------------------------------------------===//
void test_new_launder() {
    int x = 42;
    int* p = std::launder(&x);
    CHECK(p == &x);
    CHECK(*p == 42);

    // Works with const pointer
    const int y = 99;
    const int* cp = std::launder(&y);
    CHECK(cp == &y);
    CHECK(*cp == 99);
}

//===----------------------------------------------------------------------===//
// test_new_destroying_delete_t
//===----------------------------------------------------------------------===//
void test_new_destroying_delete_t() {
    static_assert(__is_class(std::destroying_delete_t));

    // destroying_delete is a constexpr object of type destroying_delete_t
    static_assert(__is_same(decltype(std::destroying_delete),
                            const std::destroying_delete_t));
}

//===----------------------------------------------------------------------===//
// test_new_interference_size
//===----------------------------------------------------------------------===//
void test_new_interference_size() {
    // Both constants must be positive and power-of-two (typical cache line)
    constexpr std::size_t d = std::hardware_destructive_interference_size;
    constexpr std::size_t c = std::hardware_constructive_interference_size;
    static_assert(d > 0);
    static_assert(c > 0);
    static_assert((d & (d - 1)) == 0); // power of two
    static_assert((c & (c - 1)) == 0);
}

//===----------------------------------------------------------------------===//
// test_new_global_new_delete
// Exercises operator new / delete round-trip (plain, nothrow, aligned).
//===----------------------------------------------------------------------===//
void test_new_global_new_delete() {
    // Plain new/delete
    void* p1 = operator new(64);
    CHECK(p1 != nullptr);
    operator delete(p1);

    // Nothrow new/delete
    void* p2 = operator new(64, std::nothrow);
    // nothrow new may return null on failure, but for a small allocation it won't
    CHECK(p2 != nullptr);
    operator delete(p2, std::nothrow);

    // Aligned new/delete
    void* p3 = operator new(64, std::align_val_t(64));
    CHECK(p3 != nullptr);
    // Verify alignment
    CHECK((reinterpret_cast<std::size_t>(p3) & 63) == 0);
    operator delete(p3, std::align_val_t(64));

    // Array new/delete
    void* p4 = operator new[](128);
    CHECK(p4 != nullptr);
    operator delete[](p4);
}

//===----------------------------------------------------------------------===//
// test_new_placement_new_delete
//===----------------------------------------------------------------------===//
void test_new_placement_new_delete() {
    alignas(int) unsigned char buf[sizeof(int)];
    void* p = operator new(sizeof(int), static_cast<void*>(buf));
    CHECK(p == static_cast<void*>(buf));

    // Placement delete is a no-op
    operator delete(p, static_cast<void*>(buf));

    // Array placement
    void* p2 = operator new[](sizeof(int), static_cast<void*>(buf));
    CHECK(p2 == static_cast<void*>(buf));
    operator delete[](p2, static_cast<void*>(buf));
}
