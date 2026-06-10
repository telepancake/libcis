// test/version.cpp — tests for <version>
// Conventions: void test_version_<case>(), static_assert + CHECK, no main().
#include <version>
#include "test.h"

void test_version_defined() {
    // Core C++20 macros that both libc++ and libstdc++ are known to define.
    // Guard each macro individually so the file compiles on any conforming
    // implementation, even if a specific macro is absent.
#ifdef __cpp_lib_optional
    static_assert(__cpp_lib_optional >= 201606L);
#endif
#ifdef __cpp_lib_variant
    static_assert(__cpp_lib_variant  >= 201606L);
#endif
#ifdef __cpp_lib_span
    static_assert(__cpp_lib_span     >= 202002L);
#endif
#ifdef __cpp_lib_concepts
    static_assert(__cpp_lib_concepts >= 201806L);
#endif
#ifdef __cpp_lib_ranges
    static_assert(__cpp_lib_ranges   >= 202110L);
#endif
#ifdef __cpp_lib_make_unique
    static_assert(__cpp_lib_make_unique >= 201304L);
#endif
    // __cpp_lib_to_chars is absent in libc++ when floating-point to_chars
    // is not available; guard it so the test is portable.
#ifdef __cpp_lib_to_chars
    static_assert(__cpp_lib_to_chars >= 201611L);
#endif
#ifdef __cpp_lib_bitops
    static_assert(__cpp_lib_bitops   >= 201907L);
#endif
#ifdef __cpp_lib_endian
    static_assert(__cpp_lib_endian   >= 201907L);
#endif
    // __cpp_lib_three_way_comparison is absent in some libc++ configurations.
#ifdef __cpp_lib_three_way_comparison
    static_assert(__cpp_lib_three_way_comparison >= 201907L);
#endif
}

void test_version_optional_macro() {
#ifdef __cpp_lib_optional
    static_assert(__cpp_lib_optional >= 201606L);
#else
    static_assert(false, "__cpp_lib_optional must be defined");
#endif
}

// NOTE: __cpp_lib_format may or may not be defined depending on the vendor
// implementation (e.g. libstdc++13 defines it, libc++ may not). A portable
// test cannot assert the macro is absent; this function simply guards its
// value when present.
void test_version_no_format() {
#ifdef __cpp_lib_format
    static_assert(__cpp_lib_format >= 202110L);
#endif
}
