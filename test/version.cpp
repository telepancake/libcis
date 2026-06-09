// test/version.cpp — tests for <version>
// Conventions: void test_version_<case>(), static_assert + CHECK, no main().
#include <version>
#include "test.h"

void test_version_defined() {
    // Core C++20 macros must be defined
    static_assert(__cpp_lib_optional >= 201606L);
    static_assert(__cpp_lib_variant  >= 201606L);
    static_assert(__cpp_lib_span     == 202002L);
    static_assert(__cpp_lib_concepts >= 201806L);
    static_assert(__cpp_lib_ranges   >= 202110L);
    static_assert(__cpp_lib_make_unique == 201304L);
    static_assert(__cpp_lib_to_chars == 201611L);
    static_assert(__cpp_lib_bitops   == 201907L);
    static_assert(__cpp_lib_endian   == 201907L);
    static_assert(__cpp_lib_three_way_comparison == 201907L);
}

void test_version_optional_macro() {
#ifdef __cpp_lib_optional
    static_assert(__cpp_lib_optional >= 201606L);
#else
    static_assert(false, "__cpp_lib_optional must be defined");
#endif
}

void test_version_no_format() {
    // <format> is not implemented in libcis; macro must NOT be defined
#ifdef __cpp_lib_format
    static_assert(false, "__cpp_lib_format must not be defined in libcis");
#endif
}
