// test/typeindex.cpp — compile-only test for <typeindex>
// type_index cannot be populated under -fno-rtti (typeid is unavailable),
// so this is purely a compile-time check that the header is well-formed
// and the declared interface is present.
#include <typeindex>
#include <functional>
#include "test.h"

// Verify the API is present and the hash specialisation compiles.
static_assert(sizeof(std::type_index) > 0, "type_index must be a complete type");

// Verify hash<type_index> is accessible.
void test_typeindex_compile_only() {
    // No runtime test possible under -fno-rtti (cannot call typeid).
    // Verify the hash template can be instantiated.
    std::hash<std::type_index> h;
    (void)h;
    // This test always passes — it is a compile-time check only.
    CHECK(true);
}
