#include <memory_resource>
#include "test.h"

void test_memory_resource_monotonic_buffer() {
    // monotonic_buffer_resource from a stack buffer
    char buf[256];
    std::pmr::monotonic_buffer_resource mbr(buf, sizeof(buf));

    // Allocate from it
    void* p1 = mbr.allocate(16, 8);
    CHECK(p1 != nullptr);

    void* p2 = mbr.allocate(32, 8);
    CHECK(p2 != nullptr);
    CHECK(p1 != p2);

    // deallocate is a no-op for monotonic — just shouldn't crash
    mbr.deallocate(p1, 16, 8);

    // release
    mbr.release();
}

void test_memory_resource_polymorphic_allocator() {
    // polymorphic_allocator allocate/construct an int
    std::pmr::polymorphic_allocator<int> alloc;
    int* p = alloc.allocate(1);
    CHECK(p != nullptr);
    alloc.construct(p, 42);
    CHECK(*p == 42);
    alloc.destroy(p);
    alloc.deallocate(p, 1);
}

void test_memory_resource_new_delete_resource() {
    // new_delete_resource round-trip
    std::pmr::memory_resource* mr = std::pmr::new_delete_resource();
    CHECK(mr != nullptr);

    void* p = mr->allocate(64, 8);
    CHECK(p != nullptr);
    mr->deallocate(p, 64, 8);

    // is_equal to itself
    CHECK(mr->is_equal(*mr));

    // get/set default resource
    std::pmr::memory_resource* old = std::pmr::set_default_resource(mr);
    CHECK(std::pmr::get_default_resource() == mr);
    std::pmr::set_default_resource(old);
}
