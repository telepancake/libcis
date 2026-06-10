// test/bit.cpp -- tests for <bit>
// Ported from libc++ test/std/numerics/bit/
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include "test.h"

// Local replacement for the libcis-internal std::detail::bit_digits_v<T>.
// The standard does not expose this; it equals the number of value bits,
// which for unsigned integer types is CHAR_BIT * sizeof(T).
template<class T>
inline constexpr int bit_digits = static_cast<int>(sizeof(T) * CHAR_BIT);

//===----------------------------------------------------------------------===//
// endian
//===----------------------------------------------------------------------===//

void test_bit_endian() {
    // native must be either little or big on any real machine.
    static_assert(std::endian::native == std::endian::little ||
                  std::endian::native == std::endian::big,
                  "native must be little or big");
    static_assert(std::endian::little != std::endian::big, "");
    // On x86/amd64 the test machine must be little-endian.
    static_assert(std::endian::native == std::endian::little,
                  "expected little-endian host");
}

//===----------------------------------------------------------------------===//
// bit_cast
// Not constexpr (gcc-10 lacks __builtin_bit_cast), so runtime-only tests.
//===----------------------------------------------------------------------===//

void test_bit_cast() {
    // int <-> float (same size, trivially copyable)
    float f = 1.0f;
    uint32_t u = std::bit_cast<uint32_t>(f);
    float f2 = std::bit_cast<float>(u);
    CHECK(f == f2);

    // memcmp round-trip
    uint32_t raw1, raw2;
    __builtin_memcpy(&raw1, &f, 4);
    CHECK(u == raw1);
    __builtin_memcpy(&raw2, &f2, 4);
    CHECK(raw1 == raw2);

    // double <-> uint64_t
    double d = 3.14159265358979;
    uint64_t ud = std::bit_cast<uint64_t>(d);
    double d2 = std::bit_cast<double>(ud);
    CHECK(d == d2);

    // int -> struct of same size
    struct Buf4 { char b[4]; };
    int i = 0x12345678;
    Buf4 buf = std::bit_cast<Buf4>(i);
    int i2 = std::bit_cast<int>(buf);
    CHECK(i == i2);

    // pointer round-trip
    int obj = 42;
    int* p = &obj;
    using UintPtr = unsigned long; // same size as pointer on 64-bit LP64
    static_assert(sizeof(int*) == sizeof(UintPtr), "need pointer == UintPtr");
    UintPtr up = std::bit_cast<UintPtr>(p);
    int* p2 = std::bit_cast<int*>(up);
    CHECK(p == p2);
    CHECK(*p2 == 42);
}

//===----------------------------------------------------------------------===//
// countl_zero
//===----------------------------------------------------------------------===//

template<class T>
static constexpr void check_countl_zero() {
    constexpr int dig = bit_digits<T>;

    static_assert(std::countl_zero(T(0))   == dig);
    static_assert(std::countl_zero(T(1))   == dig - 1);
    static_assert(std::countl_zero(T(2))   == dig - 2);
    static_assert(std::countl_zero(T(3))   == dig - 2);
    static_assert(std::countl_zero(T(4))   == dig - 3);
    static_assert(std::countl_zero(T(128)) == dig - 8);
    static_assert(std::countl_zero(T(~T(0))) == 0);
}

void test_bit_countl_zero() {
    check_countl_zero<unsigned char>();
    check_countl_zero<unsigned short>();
    check_countl_zero<unsigned int>();
    check_countl_zero<unsigned long>();
    check_countl_zero<unsigned long long>();
    check_countl_zero<uint8_t>();
    check_countl_zero<uint16_t>();
    check_countl_zero<uint32_t>();
    check_countl_zero<uint64_t>();

    // Runtime checks
    CHECK(std::countl_zero(0u) == 32);
    CHECK(std::countl_zero(1u) == 31);
    CHECK(std::countl_zero(0x80000000u) == 0);
    CHECK(std::countl_zero(0xFFFFFFFFu) == 0);
    CHECK(std::countl_zero(0ULL) == 64);
    CHECK(std::countl_zero(1ULL) == 63);
    CHECK(std::countl_zero(~0ULL) == 0);

    // __uint128_t
    __uint128_t z = 0;
    __uint128_t one = 1;
    CHECK(std::countl_zero(z) == 128);
    CHECK(std::countl_zero(one) == 127);
    CHECK(std::countl_zero(one << 64) == 63);
    CHECK(std::countl_zero(one << 127) == 0);
    CHECK(std::countl_zero(~z) == 0);
}

//===----------------------------------------------------------------------===//
// countl_one
//===----------------------------------------------------------------------===//

void test_bit_countl_one() {
    CHECK(std::countl_one(0u)          == 0);
    CHECK(std::countl_one(~0u)         == 32);
    CHECK(std::countl_one(0x80000000u) == 1);
    CHECK(std::countl_one(0xFFFF0000u) == 16);
    CHECK(std::countl_one(0ULL)        == 0);
    CHECK(std::countl_one(~0ULL)       == 64);
}

//===----------------------------------------------------------------------===//
// countr_zero
//===----------------------------------------------------------------------===//

template<class T>
static constexpr void check_countr_zero() {
    constexpr int dig = bit_digits<T>;
    static_assert(std::countr_zero(T(0)) == dig);
    static_assert(std::countr_zero(T(1)) == 0);
    static_assert(std::countr_zero(T(2)) == 1);
    static_assert(std::countr_zero(T(4)) == 2);
    static_assert(std::countr_zero(T(8)) == 3);
    static_assert(std::countr_zero(T(~T(0))) == 0);
}

void test_bit_countr_zero() {
    check_countr_zero<unsigned char>();
    check_countr_zero<unsigned short>();
    check_countr_zero<unsigned int>();
    check_countr_zero<unsigned long>();
    check_countr_zero<unsigned long long>();

    CHECK(std::countr_zero(0u)          == 32);
    CHECK(std::countr_zero(1u)          == 0);
    CHECK(std::countr_zero(2u)          == 1);
    CHECK(std::countr_zero(0x100u)      == 8);
    CHECK(std::countr_zero(~0u)         == 0);
    CHECK(std::countr_zero(0ULL)        == 64);
    CHECK(std::countr_zero(1ULL)        == 0);
    CHECK(std::countr_zero(~0ULL)       == 0);

    __uint128_t z = 0;
    __uint128_t one = 1;
    CHECK(std::countr_zero(z)        == 128);
    CHECK(std::countr_zero(one)      == 0);
    CHECK(std::countr_zero(one << 64)== 64);
    CHECK(std::countr_zero(one << 127)== 127);
    CHECK(std::countr_zero(~z)       == 0);
}

//===----------------------------------------------------------------------===//
// countr_one
//===----------------------------------------------------------------------===//

void test_bit_countr_one() {
    CHECK(std::countr_one(0u)          == 0);
    CHECK(std::countr_one(1u)          == 1);
    CHECK(std::countr_one(3u)          == 2);
    CHECK(std::countr_one(7u)          == 3);
    CHECK(std::countr_one(~0u)         == 32);
    CHECK(std::countr_one(0xFFFFFFF0u) == 0);
    CHECK(std::countr_one(0xFFFFFFFFu) == 32);
}

//===----------------------------------------------------------------------===//
// popcount
//===----------------------------------------------------------------------===//

template<class T>
static constexpr void check_popcount() {
    static_assert(std::popcount(T(0)) == 0);
    static_assert(std::popcount(T(1)) == 1);
    static_assert(std::popcount(T(2)) == 1);
    static_assert(std::popcount(T(3)) == 2);
    static_assert(std::popcount(T(~T(0))) == bit_digits<T>);
}

void test_bit_popcount() {
    check_popcount<unsigned char>();
    check_popcount<unsigned short>();
    check_popcount<unsigned int>();
    check_popcount<unsigned long>();
    check_popcount<unsigned long long>();

    CHECK(std::popcount(0u)          == 0);
    CHECK(std::popcount(1u)          == 1);
    CHECK(std::popcount(0xFFFFFFFFu) == 32);
    CHECK(std::popcount(0x55555555u) == 16);
    CHECK(std::popcount(~0ULL)       == 64);
    CHECK(std::popcount(0ULL)        == 0);

    __uint128_t all = ~__uint128_t(0);
    CHECK(std::popcount(all)        == 128);
    CHECK(std::popcount(__uint128_t(0)) == 0);
    CHECK(std::popcount(__uint128_t(1) | (__uint128_t(1) << 64)) == 2);
}

//===----------------------------------------------------------------------===//
// rotl / rotr
//===----------------------------------------------------------------------===//

void test_bit_rotl() {
    static_assert(std::rotl(1u, 0) == 1u);
    static_assert(std::rotl(1u, 1) == 2u);
    static_assert(std::rotl(1u, 4) == 16u);
    static_assert(std::rotl(0x80000000u, 1) == 1u);
    static_assert(std::rotl(~0u - 1, 0) == ~0u - 1);

    CHECK(std::rotl(1u, 0)  == 1u);
    CHECK(std::rotl(1u, 1)  == 2u);
    CHECK(std::rotl(1u, 31) == 0x80000000u);
    CHECK(std::rotl(1u, 32) == 1u);  // full rotation
    CHECK(std::rotl(0x80000000u, 1) == 1u);  // wrap-around
    // Negative rotation = rotr
    CHECK(std::rotl(1u, -1) == 0x80000000u);
    CHECK(std::rotl(128u, -1) == 64u);
    CHECK(std::rotl(128u, -7) == 1u);

    // 64-bit
    CHECK(std::rotl(1ULL, 0)  == 1ULL);
    CHECK(std::rotl(1ULL, 63) == (1ULL << 63));
    CHECK(std::rotl(1ULL << 63, 1) == 1ULL);  // wrap

    // __uint128_t
    __uint128_t one128 = 1;
    CHECK(std::rotl(one128, 0)   == one128);
    CHECK(std::rotl(one128, 127) == (one128 << 127));
    CHECK(std::rotl(one128 << 127, 1) == one128);
    CHECK(std::rotl(one128, 128) == one128);
}

void test_bit_rotr() {
    static_assert(std::rotr(1u, 0) == 1u);
    static_assert(std::rotr(1u, 1) == 0x80000000u);
    static_assert(std::rotr(16u, 4) == 1u);
    static_assert(std::rotr(~0u - 1, 0) == ~0u - 1);

    CHECK(std::rotr(1u, 0)  == 1u);
    CHECK(std::rotr(1u, 1)  == 0x80000000u);
    CHECK(std::rotr(1u, 32) == 1u);  // full rotation
    CHECK(std::rotr(16u, 4) == 1u);
    CHECK(std::rotr(1u, -1) == 2u);  // negative = rotl
}

//===----------------------------------------------------------------------===//
// has_single_bit
//===----------------------------------------------------------------------===//

void test_bit_has_single_bit() {
    static_assert(!std::has_single_bit(0u));
    static_assert( std::has_single_bit(1u));
    static_assert( std::has_single_bit(2u));
    static_assert(!std::has_single_bit(3u));
    static_assert( std::has_single_bit(4u));
    static_assert(!std::has_single_bit(7u));
    static_assert( std::has_single_bit(8u));
    static_assert(!std::has_single_bit(~0u));

    CHECK(!std::has_single_bit(0u));
    CHECK( std::has_single_bit(1u));
    CHECK( std::has_single_bit(128u));
    CHECK(!std::has_single_bit(127u));
    CHECK(!std::has_single_bit(~0u));

    __uint128_t one128 = 1;
    CHECK( std::has_single_bit(one128));
    CHECK( std::has_single_bit(one128 << 64));
    CHECK( std::has_single_bit(one128 << 127));
    CHECK(!std::has_single_bit(one128 | (one128 << 64)));
    CHECK(!std::has_single_bit(__uint128_t(0)));
}

//===----------------------------------------------------------------------===//
// bit_floor
//===----------------------------------------------------------------------===//

void test_bit_bit_floor() {
    static_assert(std::bit_floor(0u)   == 0u);
    static_assert(std::bit_floor(1u)   == 1u);
    static_assert(std::bit_floor(2u)   == 2u);
    static_assert(std::bit_floor(3u)   == 2u);
    static_assert(std::bit_floor(4u)   == 4u);
    static_assert(std::bit_floor(5u)   == 4u);
    static_assert(std::bit_floor(8u)   == 8u);
    static_assert(std::bit_floor(9u)   == 8u);
    static_assert(std::bit_floor(127u) == 64u);
    static_assert(std::bit_floor(128u) == 128u);

    CHECK(std::bit_floor(0u)   == 0u);
    CHECK(std::bit_floor(1u)   == 1u);
    CHECK(std::bit_floor(2u)   == 2u);
    CHECK(std::bit_floor(3u)   == 2u);
    CHECK(std::bit_floor(128u) == 128u);
    CHECK(std::bit_floor(129u) == 128u);
    CHECK(std::bit_floor(0x80000000u) == 0x80000000u);
    CHECK(std::bit_floor(~0u) == 0x80000000u);
}

//===----------------------------------------------------------------------===//
// bit_width
//===----------------------------------------------------------------------===//

void test_bit_bit_width() {
    static_assert(std::bit_width(0u)   == 0);
    static_assert(std::bit_width(1u)   == 1);
    static_assert(std::bit_width(2u)   == 2);
    static_assert(std::bit_width(3u)   == 2);
    static_assert(std::bit_width(4u)   == 3);
    static_assert(std::bit_width(7u)   == 3);
    static_assert(std::bit_width(8u)   == 4);
    static_assert(std::bit_width(127u) == 7);
    static_assert(std::bit_width(128u) == 8);
    static_assert(std::bit_width(~0u)  == 32);

    CHECK(std::bit_width(0u)   == 0);
    CHECK(std::bit_width(1u)   == 1);
    CHECK(std::bit_width(2u)   == 2);
    CHECK(std::bit_width(3u)   == 2);
    CHECK(std::bit_width(4u)   == 3);
    CHECK(std::bit_width(128u) == 8);
    CHECK(std::bit_width(~0u)  == 32);
    CHECK(std::bit_width(~0ULL)== 64);
}

//===----------------------------------------------------------------------===//
// bit_ceil
//===----------------------------------------------------------------------===//

void test_bit_bit_ceil() {
    static_assert(std::bit_ceil(0u)   == 1u);
    static_assert(std::bit_ceil(1u)   == 1u);
    static_assert(std::bit_ceil(2u)   == 2u);
    static_assert(std::bit_ceil(3u)   == 4u);
    static_assert(std::bit_ceil(4u)   == 4u);
    static_assert(std::bit_ceil(5u)   == 8u);
    static_assert(std::bit_ceil(7u)   == 8u);
    static_assert(std::bit_ceil(8u)   == 8u);
    static_assert(std::bit_ceil(9u)   == 16u);
    static_assert(std::bit_ceil(128u) == 128u);
    static_assert(std::bit_ceil(129u) == 256u);

    CHECK(std::bit_ceil(0u)   == 1u);
    CHECK(std::bit_ceil(1u)   == 1u);
    CHECK(std::bit_ceil(2u)   == 2u);
    CHECK(std::bit_ceil(3u)   == 4u);
    CHECK(std::bit_ceil(4u)   == 4u);
    CHECK(std::bit_ceil(5u)   == 8u);
    CHECK(std::bit_ceil(7u)   == 8u);
    CHECK(std::bit_ceil(8u)   == 8u);
    CHECK(std::bit_ceil(129u) == 256u);
    CHECK(std::bit_ceil(0x80000000u) == 0x80000000u);

    // Small types (uses the extra-bits path)
    static_assert(std::bit_ceil((uint8_t)0)   == (uint8_t)1);
    static_assert(std::bit_ceil((uint8_t)1)   == (uint8_t)1);
    static_assert(std::bit_ceil((uint8_t)3)   == (uint8_t)4);
    static_assert(std::bit_ceil((uint8_t)128) == (uint8_t)128);

    CHECK(std::bit_ceil((uint8_t)0)   == (uint8_t)1);
    CHECK(std::bit_ceil((uint8_t)3)   == (uint8_t)4);
    CHECK(std::bit_ceil((uint8_t)128) == (uint8_t)128);
}

//===----------------------------------------------------------------------===//
// byteswap
//===----------------------------------------------------------------------===//

void test_bit_byteswap() {
    static_assert(std::byteswap((uint8_t)0x12) == (uint8_t)0x12);
    static_assert(std::byteswap((uint16_t)0x1234) == (uint16_t)0x3412);
    static_assert(std::byteswap((uint32_t)0x12345678u) == (uint32_t)0x78563412u);
    static_assert(std::byteswap((uint64_t)0x123456789ABCDEF0ULL) ==
                  (uint64_t)0xF0DEBC9A78563412ULL);

    CHECK(std::byteswap((uint8_t)0x12)  == (uint8_t)0x12);
    CHECK(std::byteswap((uint16_t)0x1234) == (uint16_t)0x3412);
    CHECK(std::byteswap((uint32_t)0x12345678u) == (uint32_t)0x78563412u);
    CHECK(std::byteswap((uint64_t)0x123456789ABCDEF0ULL) == (uint64_t)0xF0DEBC9A78563412ULL);

    // Signed types must also work (byteswap takes integral, not unsigned_integer)
    CHECK(std::byteswap((int16_t)0x1234) == (int16_t)0x3412);
    CHECK(std::byteswap((int32_t)0x12345678) == (int32_t)0x78563412);

    // Round-trip
    uint32_t x = 0xDEADBEEFu;
    CHECK(std::byteswap(std::byteswap(x)) == x);
    uint64_t y = 0x0102030405060708ULL;
    CHECK(std::byteswap(std::byteswap(y)) == y);
}

//===----------------------------------------------------------------------===//
// Constraint checks: verify <bit> functions accept the right types.
// std::unsigned_integer_type and std::integer_type are libcis-internal concepts
// not part of the standard; replace with standard type traits and concept checks.
// The standard requires <bit> functions to accept all unsigned integer types
// (excluding bool, char, wchar_t, char8_t, char16_t, char32_t).
//===----------------------------------------------------------------------===//

// Helper: the standard <bit> functions are constrained on unsigned integer
// types (std::is_unsigned_v && std::is_integral_v, excluding char-like types).
// We use a local concept matching the standard's intent.
template<class T>
concept standard_unsigned_integer =
    std::is_integral_v<T> &&
    std::is_unsigned_v<T> &&
    !std::is_same_v<T, bool> &&
    !std::is_same_v<T, char> &&
    !std::is_same_v<T, wchar_t> &&
    !std::is_same_v<T, char8_t> &&
    !std::is_same_v<T, char16_t> &&
    !std::is_same_v<T, char32_t> &&
    std::is_same_v<T, std::remove_cv_t<T>>;

void test_bit_constraints() {
    // Unsigned integer types accepted by the standard <bit> functions
    static_assert( standard_unsigned_integer<unsigned char>);
    static_assert( standard_unsigned_integer<unsigned short>);
    static_assert( standard_unsigned_integer<unsigned int>);
    static_assert( standard_unsigned_integer<unsigned long>);
    static_assert( standard_unsigned_integer<unsigned long long>);
    static_assert( standard_unsigned_integer<uint8_t>);
    static_assert( standard_unsigned_integer<uint16_t>);
    static_assert( standard_unsigned_integer<uint32_t>);
    static_assert( standard_unsigned_integer<uint64_t>);

    // Rejected: bool, char, wchar_t, char8_t, char16_t, char32_t, signed types
    static_assert(!standard_unsigned_integer<bool>);
    static_assert(!standard_unsigned_integer<char>);
    static_assert(!standard_unsigned_integer<wchar_t>);
    static_assert(!standard_unsigned_integer<char8_t>);
    static_assert(!standard_unsigned_integer<char16_t>);
    static_assert(!standard_unsigned_integer<char32_t>);
    static_assert(!standard_unsigned_integer<signed char>);
    static_assert(!standard_unsigned_integer<short>);
    static_assert(!standard_unsigned_integer<int>);
    static_assert(!standard_unsigned_integer<long>);
    static_assert(!standard_unsigned_integer<long long>);
    // cv-qualified: rejected (must be unqualified)
    static_assert(!standard_unsigned_integer<const unsigned int>);
    static_assert(!standard_unsigned_integer<volatile unsigned int>);

    // Verify the <bit> functions actually compile and run for these types
    static_assert(std::popcount(uint8_t(0))  == 0);
    static_assert(std::popcount(uint16_t(0)) == 0);
    static_assert(std::popcount(uint32_t(0)) == 0);
    static_assert(std::popcount(uint64_t(0)) == 0);
    static_assert(std::countl_zero(uint8_t(0))  == 8);
    static_assert(std::countl_zero(uint16_t(0)) == 16);
    static_assert(std::countr_zero(uint32_t(0)) == 32);
}
