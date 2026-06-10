// Tests for <limits>, ported from libc++ test suite.
// Each test_* function exercises one aspect via static_assert (compile-time)
// and CHECK() (runtime). No main(); it's auto-generated.
#include <limits>
#include "test.h"

//===----------------------------------------------------------------------===//
// test_limits_enums — float_round_style and float_denorm_style values
//===----------------------------------------------------------------------===//
void test_limits_enums() {
    static_assert(std::round_indeterminate       == -1);
    static_assert(std::round_toward_zero         ==  0);
    static_assert(std::round_to_nearest          ==  1);
    static_assert(std::round_toward_infinity     ==  2);
    static_assert(std::round_toward_neg_infinity ==  3);

    static_assert(std::denorm_indeterminate == -1);
    static_assert(std::denorm_absent        ==  0);
    static_assert(std::denorm_present       ==  1);
}

//===----------------------------------------------------------------------===//
// test_limits_unspecialized — non-arithmetic types are not specialized
//===----------------------------------------------------------------------===//
struct MyStruct {};

void test_limits_unspecialized() {
    static_assert(!std::numeric_limits<MyStruct>::is_specialized);
    static_assert(!std::numeric_limits<void*>::is_specialized);
}

//===----------------------------------------------------------------------===//
// test_limits_is_specialized — all arithmetic types are specialized
//===----------------------------------------------------------------------===//
void test_limits_is_specialized() {
    static_assert(std::numeric_limits<bool>::is_specialized);
    static_assert(std::numeric_limits<char>::is_specialized);
    static_assert(std::numeric_limits<signed char>::is_specialized);
    static_assert(std::numeric_limits<unsigned char>::is_specialized);
    static_assert(std::numeric_limits<wchar_t>::is_specialized);
    static_assert(std::numeric_limits<char8_t>::is_specialized);
    static_assert(std::numeric_limits<char16_t>::is_specialized);
    static_assert(std::numeric_limits<char32_t>::is_specialized);
    static_assert(std::numeric_limits<short>::is_specialized);
    static_assert(std::numeric_limits<unsigned short>::is_specialized);
    static_assert(std::numeric_limits<int>::is_specialized);
    static_assert(std::numeric_limits<unsigned int>::is_specialized);
    static_assert(std::numeric_limits<long>::is_specialized);
    static_assert(std::numeric_limits<unsigned long>::is_specialized);
    static_assert(std::numeric_limits<long long>::is_specialized);
    static_assert(std::numeric_limits<unsigned long long>::is_specialized);
    static_assert(std::numeric_limits<__int128>::is_specialized);
    static_assert(std::numeric_limits<unsigned __int128>::is_specialized);
    static_assert(std::numeric_limits<float>::is_specialized);
    static_assert(std::numeric_limits<double>::is_specialized);
    static_assert(std::numeric_limits<long double>::is_specialized);

    // CV-qualified wrappers inherit the same value.
    static_assert(std::numeric_limits<const int>::is_specialized);
    static_assert(std::numeric_limits<volatile int>::is_specialized);
    static_assert(std::numeric_limits<const volatile int>::is_specialized);
    static_assert(std::numeric_limits<const float>::is_specialized);
}

//===----------------------------------------------------------------------===//
// test_limits_is_signed — signed-ness of each type
//===----------------------------------------------------------------------===//
void test_limits_is_signed() {
    static_assert(!std::numeric_limits<bool>::is_signed);
    static_assert(std::numeric_limits<signed char>::is_signed);
    static_assert(!std::numeric_limits<unsigned char>::is_signed);
    static_assert(!std::numeric_limits<char8_t>::is_signed);
    static_assert(!std::numeric_limits<char16_t>::is_signed);
    static_assert(!std::numeric_limits<char32_t>::is_signed);
    static_assert(std::numeric_limits<short>::is_signed);
    static_assert(!std::numeric_limits<unsigned short>::is_signed);
    static_assert(std::numeric_limits<int>::is_signed);
    static_assert(!std::numeric_limits<unsigned int>::is_signed);
    static_assert(std::numeric_limits<long>::is_signed);
    static_assert(!std::numeric_limits<unsigned long>::is_signed);
    static_assert(std::numeric_limits<long long>::is_signed);
    static_assert(!std::numeric_limits<unsigned long long>::is_signed);
    static_assert(std::numeric_limits<__int128>::is_signed);
    static_assert(!std::numeric_limits<unsigned __int128>::is_signed);
    static_assert(std::numeric_limits<float>::is_signed);
    static_assert(std::numeric_limits<double>::is_signed);
    static_assert(std::numeric_limits<long double>::is_signed);

    // char signedness depends on the platform; just check consistency.
    static_assert(std::numeric_limits<char>::is_signed == (char(-1) < char(0)));
}

//===----------------------------------------------------------------------===//
// test_limits_digits — bit-widths (mantissa/value bits)
//===----------------------------------------------------------------------===//
void test_limits_digits() {
    static_assert(std::numeric_limits<bool>::digits == 1);
    static_assert(std::numeric_limits<signed char>::digits == 7);
    static_assert(std::numeric_limits<unsigned char>::digits == 8);
    static_assert(std::numeric_limits<short>::digits == 15);
    static_assert(std::numeric_limits<unsigned short>::digits == 16);
    static_assert(std::numeric_limits<int>::digits == 31);
    static_assert(std::numeric_limits<unsigned int>::digits == 32);
    static_assert(std::numeric_limits<long long>::digits == 63);
    static_assert(std::numeric_limits<unsigned long long>::digits == 64);
    static_assert(std::numeric_limits<__int128>::digits == 127);
    static_assert(std::numeric_limits<unsigned __int128>::digits == 128);
    static_assert(std::numeric_limits<float>::digits == __FLT_MANT_DIG__);
    static_assert(std::numeric_limits<double>::digits == __DBL_MANT_DIG__);
    static_assert(std::numeric_limits<long double>::digits == __LDBL_MANT_DIG__);

    // char: depends on signedness
    static_assert(std::numeric_limits<char>::digits ==
        (std::numeric_limits<char>::is_signed ? 7 : 8));
}

//===----------------------------------------------------------------------===//
// test_limits_digits10
//===----------------------------------------------------------------------===//
void test_limits_digits10() {
    static_assert(std::numeric_limits<bool>::digits10 == 0);
    static_assert(std::numeric_limits<signed char>::digits10 == 2);
    static_assert(std::numeric_limits<unsigned char>::digits10 == 2);
    static_assert(std::numeric_limits<short>::digits10 == 4);
    static_assert(std::numeric_limits<unsigned short>::digits10 == 4);
    static_assert(std::numeric_limits<int>::digits10 == 9);
    static_assert(std::numeric_limits<unsigned int>::digits10 == 9);
    static_assert(std::numeric_limits<long long>::digits10 == 18);
    static_assert(std::numeric_limits<unsigned long long>::digits10 == 19);
    static_assert(std::numeric_limits<float>::digits10 == __FLT_DIG__);
    static_assert(std::numeric_limits<double>::digits10 == __DBL_DIG__);
    static_assert(std::numeric_limits<long double>::digits10 == __LDBL_DIG__);
}

//===----------------------------------------------------------------------===//
// test_limits_min — minimum representable value
//===----------------------------------------------------------------------===//
void test_limits_min() {
    // Integers: min for unsigned == 0, signed uses two's-complement min.
    static_assert(std::numeric_limits<bool>::min()          == false);
    static_assert(std::numeric_limits<unsigned char>::min() == 0);
    static_assert(std::numeric_limits<unsigned short>::min()== 0);
    static_assert(std::numeric_limits<unsigned int>::min()  == 0);
    static_assert(std::numeric_limits<unsigned long>::min() == 0);
    static_assert(std::numeric_limits<unsigned long long>::min() == 0);

    // These rely on the system limit macros indirectly; just verify runtime.
    CHECK(std::numeric_limits<signed char>::min()  == -128);
    CHECK(std::numeric_limits<short>::min()        == -32768);
    CHECK(std::numeric_limits<int>::min()          == -2147483648);
    CHECK(std::numeric_limits<long long>::min()    == (-9223372036854775807LL - 1));

    // Float: min is smallest positive normal number.
    CHECK(std::numeric_limits<float>::min()       == __FLT_MIN__);
    CHECK(std::numeric_limits<double>::min()      == __DBL_MIN__);
    CHECK(std::numeric_limits<long double>::min() == __LDBL_MIN__);

    // CV-qualified versions inherit the same value.
    static_assert(std::numeric_limits<const int>::min()          == std::numeric_limits<int>::min());
    static_assert(std::numeric_limits<volatile int>::min()       == std::numeric_limits<int>::min());
    static_assert(std::numeric_limits<const volatile int>::min() == std::numeric_limits<int>::min());
}

//===----------------------------------------------------------------------===//
// test_limits_max — maximum representable value
//===----------------------------------------------------------------------===//
void test_limits_max() {
    static_assert(std::numeric_limits<bool>::max()           == true);
    static_assert(std::numeric_limits<unsigned char>::max()  == 255);
    static_assert(std::numeric_limits<signed char>::max()    == 127);
    static_assert(std::numeric_limits<unsigned short>::max() == 65535);
    static_assert(std::numeric_limits<short>::max()          == 32767);
    static_assert(std::numeric_limits<unsigned int>::max()   == 4294967295U);
    static_assert(std::numeric_limits<int>::max()            == 2147483647);
    static_assert(std::numeric_limits<unsigned long long>::max() == 18446744073709551615ULL);
    static_assert(std::numeric_limits<long long>::max()     == 9223372036854775807LL);

    // unsigned __int128
    static_assert(std::numeric_limits<unsigned __int128>::max() ==
        static_cast<unsigned __int128>(-1));
    // __int128
    static_assert(std::numeric_limits<__int128>::max() ==
        static_cast<__int128>(static_cast<unsigned __int128>(-1) >> 1));

    CHECK(std::numeric_limits<float>::max()       == __FLT_MAX__);
    CHECK(std::numeric_limits<double>::max()      == __DBL_MAX__);
    CHECK(std::numeric_limits<long double>::max() == __LDBL_MAX__);

    // is_bounded must be true for all specialized types.
    static_assert(std::numeric_limits<int>::is_bounded);
    static_assert(std::numeric_limits<float>::is_bounded);
    static_assert(std::numeric_limits<const double>::is_bounded);
}

//===----------------------------------------------------------------------===//
// test_limits_lowest — lowest (most negative) representable value
//===----------------------------------------------------------------------===//
void test_limits_lowest() {
    // For integers lowest() == min().
    static_assert(std::numeric_limits<int>::lowest()  == std::numeric_limits<int>::min());
    static_assert(std::numeric_limits<bool>::lowest() == std::numeric_limits<bool>::min());

    // For floats lowest() == -max().
    CHECK(std::numeric_limits<float>::lowest()       == -__FLT_MAX__);
    CHECK(std::numeric_limits<double>::lowest()      == -__DBL_MAX__);
    CHECK(std::numeric_limits<long double>::lowest() == -__LDBL_MAX__);
}

//===----------------------------------------------------------------------===//
// test_limits_integer_properties — is_integer, is_exact, radix, is_modulo
//===----------------------------------------------------------------------===//
void test_limits_integer_properties() {
    // Integer types
    static_assert(std::numeric_limits<bool>::is_integer);
    static_assert(std::numeric_limits<int>::is_integer);
    static_assert(std::numeric_limits<unsigned long long>::is_integer);
    static_assert(std::numeric_limits<__int128>::is_integer);

    static_assert(std::numeric_limits<bool>::is_exact);
    static_assert(std::numeric_limits<int>::is_exact);

    static_assert(std::numeric_limits<int>::radix    == 2);
    static_assert(std::numeric_limits<bool>::radix   == 2);

    // Unsigned types are modulo.
    static_assert(std::numeric_limits<unsigned int>::is_modulo);
    static_assert(std::numeric_limits<unsigned long long>::is_modulo);
    static_assert(!std::numeric_limits<int>::is_modulo);
    static_assert(!std::numeric_limits<bool>::is_modulo);

    // Floating-point types
    static_assert(!std::numeric_limits<float>::is_integer);
    static_assert(!std::numeric_limits<float>::is_exact);
    static_assert(!std::numeric_limits<float>::is_modulo);
    static_assert(std::numeric_limits<float>::radix == __FLT_RADIX__);
}

//===----------------------------------------------------------------------===//
// test_limits_epsilon_round_error
//===----------------------------------------------------------------------===//
void test_limits_epsilon_round_error() {
    static_assert(std::numeric_limits<int>::epsilon()     == 0);
    static_assert(std::numeric_limits<int>::round_error() == 0);

    CHECK(std::numeric_limits<float>::epsilon()     == __FLT_EPSILON__);
    CHECK(std::numeric_limits<double>::epsilon()    == __DBL_EPSILON__);
    CHECK(std::numeric_limits<float>::round_error() == 0.5F);
    CHECK(std::numeric_limits<double>::round_error()== 0.5);
}

//===----------------------------------------------------------------------===//
// test_limits_exponents — min/max exponents for floating-point types
//===----------------------------------------------------------------------===//
void test_limits_exponents() {
    static_assert(std::numeric_limits<int>::min_exponent    == 0);
    static_assert(std::numeric_limits<int>::max_exponent    == 0);
    static_assert(std::numeric_limits<int>::min_exponent10  == 0);
    static_assert(std::numeric_limits<int>::max_exponent10  == 0);

    static_assert(std::numeric_limits<float>::min_exponent    == __FLT_MIN_EXP__);
    static_assert(std::numeric_limits<float>::max_exponent    == __FLT_MAX_EXP__);
    static_assert(std::numeric_limits<float>::min_exponent10  == __FLT_MIN_10_EXP__);
    static_assert(std::numeric_limits<float>::max_exponent10  == __FLT_MAX_10_EXP__);

    static_assert(std::numeric_limits<double>::min_exponent   == __DBL_MIN_EXP__);
    static_assert(std::numeric_limits<double>::max_exponent   == __DBL_MAX_EXP__);
    static_assert(std::numeric_limits<double>::min_exponent10 == __DBL_MIN_10_EXP__);
    static_assert(std::numeric_limits<double>::max_exponent10 == __DBL_MAX_10_EXP__);

    static_assert(std::numeric_limits<long double>::min_exponent   == __LDBL_MIN_EXP__);
    static_assert(std::numeric_limits<long double>::max_exponent   == __LDBL_MAX_EXP__);
    static_assert(std::numeric_limits<long double>::min_exponent10 == __LDBL_MIN_10_EXP__);
    static_assert(std::numeric_limits<long double>::max_exponent10 == __LDBL_MAX_10_EXP__);
}

//===----------------------------------------------------------------------===//
// test_limits_infinity — infinity() and has_infinity
//===----------------------------------------------------------------------===//
void test_limits_infinity() {
    // Integer types have no infinity.
    static_assert(!std::numeric_limits<int>::has_infinity);
    static_assert(std::numeric_limits<int>::infinity() == 0);

    // Float types do.
    static_assert(std::numeric_limits<float>::has_infinity);
    static_assert(std::numeric_limits<double>::has_infinity);
    static_assert(std::numeric_limits<long double>::has_infinity);

    // Runtime: infinity > max finite value.
    CHECK(std::numeric_limits<float>::infinity() > std::numeric_limits<float>::max());
    CHECK(std::numeric_limits<double>::infinity() > std::numeric_limits<double>::max());

    // CV-qualified
    static_assert(std::numeric_limits<const float>::has_infinity);
    static_assert(std::numeric_limits<volatile double>::has_infinity);
}

//===----------------------------------------------------------------------===//
// test_limits_nan — quiet_NaN, signaling_NaN, has_quiet/signaling_NaN
//===----------------------------------------------------------------------===//
void test_limits_nan() {
    static_assert(!std::numeric_limits<int>::has_quiet_NaN);
    static_assert(!std::numeric_limits<int>::has_signaling_NaN);

    static_assert(std::numeric_limits<float>::has_quiet_NaN);
    static_assert(std::numeric_limits<float>::has_signaling_NaN);
    static_assert(std::numeric_limits<double>::has_quiet_NaN);
    static_assert(std::numeric_limits<double>::has_signaling_NaN);
    static_assert(std::numeric_limits<long double>::has_quiet_NaN);
    static_assert(std::numeric_limits<long double>::has_signaling_NaN);

    // NaN != NaN
    float  qf = std::numeric_limits<float>::quiet_NaN();
    double qd = std::numeric_limits<double>::quiet_NaN();
    CHECK(qf != qf);
    CHECK(qd != qd);

    // Integer NaN methods return 0.
    static_assert(std::numeric_limits<int>::quiet_NaN() == 0);
    static_assert(std::numeric_limits<int>::signaling_NaN() == 0);
}

//===----------------------------------------------------------------------===//
// test_limits_denorm — denorm_min, has_denorm, has_denorm_loss
//===----------------------------------------------------------------------===//
void test_limits_denorm() {
    static_assert(std::numeric_limits<int>::has_denorm    == std::denorm_absent);
    static_assert(std::numeric_limits<float>::has_denorm  == std::denorm_present);
    static_assert(std::numeric_limits<double>::has_denorm == std::denorm_present);

    static_assert(!std::numeric_limits<int>::has_denorm_loss);
    static_assert(!std::numeric_limits<float>::has_denorm_loss);

    // denorm_min for integers == 0.
    static_assert(std::numeric_limits<int>::denorm_min() == 0);

    // denorm_min for floats is positive and < min.
    CHECK(std::numeric_limits<float>::denorm_min() > 0.0F);
    CHECK(std::numeric_limits<float>::denorm_min() < std::numeric_limits<float>::min());
    CHECK(std::numeric_limits<double>::denorm_min() > 0.0);
    CHECK(std::numeric_limits<double>::denorm_min() < std::numeric_limits<double>::min());
}

//===----------------------------------------------------------------------===//
// test_limits_iec559 — is_iec559 (IEEE 754)
//===----------------------------------------------------------------------===//
void test_limits_iec559() {
    static_assert(!std::numeric_limits<int>::is_iec559);
    static_assert(!std::numeric_limits<bool>::is_iec559);
    static_assert(std::numeric_limits<float>::is_iec559);
    static_assert(std::numeric_limits<double>::is_iec559);
    // long double is IEEE on most platforms (not IBM double-double powerpc).
    // We just verify the type is consistent.
    static_assert(std::numeric_limits<const float>::is_iec559);
}

//===----------------------------------------------------------------------===//
// test_limits_round_style — float_round_style members
//===----------------------------------------------------------------------===//
void test_limits_round_style() {
    static_assert(std::numeric_limits<int>::round_style   == std::round_toward_zero);
    static_assert(std::numeric_limits<float>::round_style == std::round_to_nearest);
    static_assert(std::numeric_limits<double>::round_style== std::round_to_nearest);
    static_assert(std::numeric_limits<long double>::round_style == std::round_to_nearest);
}

//===----------------------------------------------------------------------===//
// test_limits_max_digits10 — decimal digits needed for round-trip
//===----------------------------------------------------------------------===//
void test_limits_max_digits10() {
    static_assert(std::numeric_limits<int>::max_digits10 == 0);
    static_assert(std::numeric_limits<bool>::max_digits10 == 0);
    // float: 2 + floor(24 * 30103 / 100000) = 2 + 7 = 9
    static_assert(std::numeric_limits<float>::max_digits10 == 9);
    // double: 2 + floor(53 * 30103 / 100000) = 2 + 15 = 17
    static_assert(std::numeric_limits<double>::max_digits10 == 17);
}

//===----------------------------------------------------------------------===//
// test_limits_cv_qualified — CV specializations equal unqualified
//===----------------------------------------------------------------------===//
void test_limits_cv_qualified() {
    static_assert(std::numeric_limits<const int>::digits       == std::numeric_limits<int>::digits);
    static_assert(std::numeric_limits<volatile int>::digits    == std::numeric_limits<int>::digits);
    static_assert(std::numeric_limits<const volatile int>::digits == std::numeric_limits<int>::digits);
    static_assert(std::numeric_limits<const float>::digits     == std::numeric_limits<float>::digits);
    static_assert(std::numeric_limits<volatile double>::digits == std::numeric_limits<double>::digits);
    static_assert(std::numeric_limits<const float>::is_iec559);
    static_assert(std::numeric_limits<volatile float>::has_infinity);
    static_assert(std::numeric_limits<const volatile double>::has_quiet_NaN);

    CHECK(std::numeric_limits<const float>::min()         == std::numeric_limits<float>::min());
    CHECK(std::numeric_limits<volatile float>::max()      == std::numeric_limits<float>::max());
    CHECK(std::numeric_limits<const volatile float>::epsilon() == std::numeric_limits<float>::epsilon());
}

//===----------------------------------------------------------------------===//
// test_limits_traps — integer trapping (implementation-defined)
//===----------------------------------------------------------------------===//
void test_limits_traps() {
    // traps is implementation-defined (e.g. true for integers on x86 vendors,
    // false for floats everywhere). Only assert the invariants the standard
    // guarantees: (a) the member exists and is a bool-contextible constant,
    // (b) float types never trap on IEEE-754 platforms.
    static_assert(!std::numeric_limits<float>::traps);
    static_assert(!std::numeric_limits<double>::traps);
    static_assert(!std::numeric_limits<long double>::traps);

    // Verify the member is accessible and boolean-typed for all integer types.
    (void)std::numeric_limits<bool>::traps;
    (void)std::numeric_limits<char>::traps;
    (void)std::numeric_limits<short>::traps;
    (void)std::numeric_limits<int>::traps;
    (void)std::numeric_limits<long>::traps;
    (void)std::numeric_limits<long long>::traps;
    (void)std::numeric_limits<unsigned int>::traps;

#if defined(__i386__) || defined(__x86_64__)
    // On x86, both libc++ and libstdc++ report traps==true for integer types
    // that are directly operated on (int, long, long long), since integer
    // divide-by-zero raises SIGFPE. Narrower types (char, short) that are
    // promoted before arithmetic also report traps==true on libstdc++.
    static_assert(std::numeric_limits<int>::traps);
    static_assert(std::numeric_limits<long>::traps);
    static_assert(std::numeric_limits<long long>::traps);
#endif
}
