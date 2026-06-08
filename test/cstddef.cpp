// Tests for <cstddef>, ported from libc++ test suite.
#include <cstddef>
#include <type_traits>
#include "test.h"

//===----------------------------------------------------------------------===//
// test_size_t
//===----------------------------------------------------------------------===//
void test_size_t() {
    static_assert(std::is_integral_v<std::size_t>);
    static_assert(std::is_unsigned_v<std::size_t>);
    static_assert(sizeof(std::size_t) == sizeof(void*));
}

//===----------------------------------------------------------------------===//
// test_ptrdiff_t
//===----------------------------------------------------------------------===//
void test_ptrdiff_t() {
    static_assert(std::is_integral_v<std::ptrdiff_t>);
    static_assert(std::is_signed_v<std::ptrdiff_t>);
    static_assert(sizeof(std::ptrdiff_t) == sizeof(void*));
}

//===----------------------------------------------------------------------===//
// test_nullptr_t
//===----------------------------------------------------------------------===//
void test_nullptr_t() {
    static_assert(std::is_null_pointer_v<std::nullptr_t>);
    std::nullptr_t np = nullptr;
    CHECK(np == nullptr);
}

//===----------------------------------------------------------------------===//
// test_max_align_t
//===----------------------------------------------------------------------===//
void test_max_align_t() {
    // max_align_t should have alignment at least as large as any fundamental type
    static_assert(alignof(std::max_align_t) >= alignof(long long));
    static_assert(alignof(std::max_align_t) >= alignof(double));
}

//===----------------------------------------------------------------------===//
// test_byte_basic
//===----------------------------------------------------------------------===//
void test_byte_basic() {
    constexpr std::byte b{42};
    static_assert(std::to_integer<int>(b) == 42);
    static_assert(std::to_integer<unsigned char>(b) == 42);

    // byte is not an integer, cannot implicitly convert
    static_assert(!std::is_integral_v<std::byte>);
    static_assert(std::is_enum_v<std::byte>);
}

//===----------------------------------------------------------------------===//
// test_byte_bitops
//===----------------------------------------------------------------------===//
void test_byte_bitops() {
    constexpr std::byte a{0b1010};
    constexpr std::byte b{0b1100};

    // OR
    static_assert(std::to_integer<int>(a | b) == 0b1110);
    // AND
    static_assert(std::to_integer<int>(a & b) == 0b1000);
    // XOR
    static_assert(std::to_integer<int>(a ^ b) == 0b0110);
    // NOT
    constexpr std::byte c{0b0000'1111};
    static_assert((std::to_integer<unsigned char>(~c) & 0xFF) ==
                  (unsigned char)(~0b0000'1111 & 0xFF));
}

//===----------------------------------------------------------------------===//
// test_byte_shift
//===----------------------------------------------------------------------===//
void test_byte_shift() {
    constexpr std::byte b{1};
    static_assert(std::to_integer<int>(b << 3) == 8);
    static_assert(std::to_integer<int>(b >> 0) == 1);

    std::byte x{0b1000};
    x >>= 2;
    CHECK(std::to_integer<int>(x) == 0b0010);

    std::byte y{1};
    y <<= 4;
    CHECK(std::to_integer<int>(y) == 16);
}

//===----------------------------------------------------------------------===//
// test_byte_compound_assign
//===----------------------------------------------------------------------===//
void test_byte_compound_assign() {
    std::byte a{0b1010};
    std::byte b{0b1100};

    std::byte c = a;
    c |= b;
    CHECK(std::to_integer<int>(c) == 0b1110);

    std::byte d = a;
    d &= b;
    CHECK(std::to_integer<int>(d) == 0b1000);

    std::byte e = a;
    e ^= b;
    CHECK(std::to_integer<int>(e) == 0b0110);
}
