// Tests for <cstdint>, <climits>, and <cfloat>.
// Each test_ function is picked up by tools/gen_main.sh.
#include "test.h"
#include <cstdint>
#include <climits>
#include <cfloat>

// ---------------------------------------------------------------------------
// <cstdint> — size checks (compile-time)
// ---------------------------------------------------------------------------
void test_cstdint_sizes() {
    static_assert(sizeof(std::int8_t)  == 1);
    static_assert(sizeof(std::int16_t) == 2);
    static_assert(sizeof(std::int32_t) == 4);
    static_assert(sizeof(std::int64_t) == 8);

    static_assert(sizeof(std::uint8_t)  == 1);
    static_assert(sizeof(std::uint16_t) == 2);
    static_assert(sizeof(std::uint32_t) == 4);
    static_assert(sizeof(std::uint64_t) == 8);
}

// ---------------------------------------------------------------------------
// <cstdint> — least/fast types exist and are at least as wide as guaranteed
// ---------------------------------------------------------------------------
void test_cstdint_least_fast_sizes() {
    static_assert(sizeof(std::int_least8_t)  >= 1);
    static_assert(sizeof(std::int_least16_t) >= 2);
    static_assert(sizeof(std::int_least32_t) >= 4);
    static_assert(sizeof(std::int_least64_t) >= 8);

    static_assert(sizeof(std::uint_least8_t)  >= 1);
    static_assert(sizeof(std::uint_least16_t) >= 2);
    static_assert(sizeof(std::uint_least32_t) >= 4);
    static_assert(sizeof(std::uint_least64_t) >= 8);

    static_assert(sizeof(std::int_fast8_t)  >= 1);
    static_assert(sizeof(std::int_fast16_t) >= 2);
    static_assert(sizeof(std::int_fast32_t) >= 4);
    static_assert(sizeof(std::int_fast64_t) >= 8);

    static_assert(sizeof(std::uint_fast8_t)  >= 1);
    static_assert(sizeof(std::uint_fast16_t) >= 2);
    static_assert(sizeof(std::uint_fast32_t) >= 4);
    static_assert(sizeof(std::uint_fast64_t) >= 8);
}

// ---------------------------------------------------------------------------
// <cstdint> — pointer-sized types
// ---------------------------------------------------------------------------
void test_cstdint_ptr_types() {
    static_assert(sizeof(std::intptr_t)  == sizeof(void*));
    static_assert(sizeof(std::uintptr_t) == sizeof(void*));
    static_assert(sizeof(std::intmax_t)  >= sizeof(std::int64_t));
    static_assert(sizeof(std::uintmax_t) >= sizeof(std::uint64_t));
}

// ---------------------------------------------------------------------------
// <cstdint> — macros present and sensible
// ---------------------------------------------------------------------------
void test_cstdint_macros() {
    static_assert(INT8_MIN  == -128);
    static_assert(INT8_MAX  == 127);
    static_assert(UINT8_MAX == 255u);

    static_assert(INT16_MIN  == -32768);
    static_assert(INT16_MAX  == 32767);
    static_assert(UINT16_MAX == 65535u);

    static_assert(INT32_MIN  == -2147483648LL);
    static_assert(INT32_MAX  == 2147483647);
    static_assert(UINT32_MAX == 4294967295u);

    static_assert(INT64_MIN  == (-9223372036854775807LL - 1));
    static_assert(INT64_MAX  == 9223372036854775807LL);
    // UINT64_MAX == 18446744073709551615ull
    static_assert(UINT64_MAX == 18446744073709551615ull);
}

// ---------------------------------------------------------------------------
// <cstdint> — runtime: values fit and round-trip
// ---------------------------------------------------------------------------
void test_cstdint_runtime_values() {
    std::int32_t a = INT32_MAX;
    std::uint32_t b = UINT32_MAX;
    CHECK(a == 2147483647);
    CHECK(b == 4294967295u);

    std::int64_t c = INT64_MIN;
    CHECK(c < 0);
    CHECK(c == (-9223372036854775807LL - 1));
}

// ---------------------------------------------------------------------------
// <climits> — core macros
// ---------------------------------------------------------------------------
void test_climits_char_bit() {
    static_assert(CHAR_BIT == 8);
}

void test_climits_int_bounds() {
    static_assert(INT_MIN  < 0);
    static_assert(INT_MAX  > 0);
    static_assert(UINT_MAX > (unsigned)INT_MAX);

    // Sanity: on all LP64/ILP32 targets CHAR_BIT==8
    static_assert(SCHAR_MIN == -128);
    static_assert(SCHAR_MAX == 127);
    static_assert(UCHAR_MAX == 255);
}

void test_climits_long_bounds() {
    static_assert(LONG_MIN < 0);
    static_assert(LONG_MAX > 0);
    static_assert(LLONG_MIN < 0);
    static_assert(LLONG_MAX > 0);
    static_assert(ULLONG_MAX >= 18446744073709551615ull);
}

// ---------------------------------------------------------------------------
// <cfloat> — basic float properties
// ---------------------------------------------------------------------------
void test_cfloat_radix() {
    // IEEE 754 mandates FLT_RADIX == 2 on all modern systems
    static_assert(FLT_RADIX == 2);
}

void test_cfloat_mant_dig() {
    // IEEE 754 single (24), double (53), extended (64 on x86)
    static_assert(FLT_MANT_DIG == 24);
    static_assert(DBL_MANT_DIG == 53);
}

void test_cfloat_max_min() {
    // Compile-time: macros must expand to constant expressions
    static_assert(FLT_MAX  > 0.0f);
    static_assert(DBL_MAX  > 0.0);
    static_assert(FLT_MIN  > 0.0f);
    static_assert(DBL_MIN  > 0.0);

    // Runtime: basic sanity
    CHECK(FLT_MAX  > 1.0f);
    CHECK(DBL_MAX  > 1.0);
    CHECK(FLT_MIN  > 0.0f);
    CHECK(FLT_MIN  < 1.0f);
}

void test_cfloat_epsilon() {
    // FLT_EPSILON > 0 and < 1
    static_assert(FLT_EPSILON > 0.0f);
    static_assert(DBL_EPSILON > 0.0);
    // 1.0 + epsilon != 1.0 (that's the definition)
    CHECK(1.0f + FLT_EPSILON != 1.0f);
    CHECK(1.0  + DBL_EPSILON != 1.0);
}
