// test/bitset.cpp — tests for <bitset>
// Ported from libc++ test/std/utilities/template.bitset/
// Target: gcc-10.2, -std=gnu++20 -fno-exceptions -fno-rtti
#include <bitset>
#include <functional>
#include <string>
#include <string_view>
#include "test.h"

// ----------------------------------------------------------------
// default constructor
// ----------------------------------------------------------------

void test_bitset_default_ctor() {
    std::bitset<0>  b0;
    std::bitset<1>  b1;
    std::bitset<8>  b8;
    std::bitset<64> b64;
    std::bitset<65> b65;
    std::bitset<128> b128;

    // All bits default to zero
    CHECK(b0.none());
    CHECK(b1.none());
    CHECK(b8.none());
    CHECK(b64.none());
    CHECK(b65.none());
    CHECK(b128.none());

    CHECK(b0.size()   == 0);
    CHECK(b1.size()   == 1);
    CHECK(b8.size()   == 8);
    CHECK(b64.size()  == 64);
    CHECK(b65.size()  == 65);
    CHECK(b128.size() == 128);

    static_assert(std::bitset<0>{}.none());
    static_assert(std::bitset<8>{}.none());
}

// ----------------------------------------------------------------
// ull constructor
// ----------------------------------------------------------------

void test_bitset_ull_ctor() {
    {
        std::bitset<0> b(0xdeadbeefULL);
        CHECK(b.none());
        CHECK(b.size() == 0);
    }
    {
        std::bitset<1> b(1ULL);
        CHECK(b[0] == true);
        CHECK(b.count() == 1);
    }
    {
        std::bitset<1> b(0ULL);
        CHECK(b[0] == false);
    }
    {
        // bits above N must be masked away
        std::bitset<4> b(0xFFULL);
        CHECK(b.count() == 4);
        CHECK(b.to_ulong() == 0xFUL);
    }
    {
        std::bitset<8> b(0xA5ULL);
        CHECK(b.to_ulong() == 0xA5UL);
    }
    {
        // 64-bit bitset from ull
        std::bitset<64> b(0xDEADBEEFCAFEBABEULL);
        CHECK(b.to_ullong() == 0xDEADBEEFCAFEBABEULL);
    }
    {
        // bitset larger than 64 bits
        std::bitset<128> b(0xFFFFFFFFFFFFFFFFULL);
        CHECK(b.to_ullong() == 0xFFFFFFFFFFFFFFFFULL);
        // high 64 bits must be 0
        for (size_t i = 64; i < 128; ++i)
            CHECK(!b.test(i));
    }
    {
        // constexpr construction
        constexpr std::bitset<8> cb(0xA5ULL);
        static_assert(cb[0] == true);
        static_assert(cb[1] == false);
        static_assert(cb[2] == true);
        static_assert(cb[7] == true);
    }
}

// ----------------------------------------------------------------
// char* constructor
// ----------------------------------------------------------------

void test_bitset_char_ptr_ctor() {
    {
        std::bitset<8> b("10110101");
        // bit 0 is rightmost char
        CHECK(b[0] == true);
        CHECK(b[1] == false);
        CHECK(b[2] == true);
        CHECK(b[3] == false);
        CHECK(b[4] == true);
        CHECK(b[5] == true);
        CHECK(b[6] == false);
        CHECK(b[7] == true);
    }
    {
        // partial string (shorter than N) — high bits zero
        std::bitset<8> b("101", 3);
        CHECK(b[0] == true);
        CHECK(b[1] == false);
        CHECK(b[2] == true);
        for (size_t i = 3; i < 8; ++i) CHECK(!b[i]);
    }
    {
        // empty string
        std::bitset<8> b("", size_t(0));
        CHECK(b.none());
    }
    {
        // custom zero/one characters
        std::bitset<4> b("ABBA", 4, 'A', 'B');
        // "ABBA" with A=0, B=1 -> bit pattern from right: A B B A
        // index 0 = rightmost = 'A' = 0
        // index 1 = 'B' = 1
        // index 2 = 'B' = 1
        // index 3 = 'A' = 0
        CHECK(!b[0]);
        CHECK(b[1]);
        CHECK(b[2]);
        CHECK(!b[3]);
    }
    {
        // string longer than N — only first N chars used
        std::bitset<4> b("11111111");
        CHECK(b.count() == 4);
    }
}

// ----------------------------------------------------------------
// string_view constructor
// std::bitset has no standard string_view constructor (C++23 still uses
// basic_string); use std::string to construct from string data.
// ----------------------------------------------------------------

void test_bitset_string_view_ctor() {
    {
        // Construct from std::string (the standard constructor)
        std::string str("10110101");
        std::bitset<8> b(str);
        CHECK(b[0] == true);
        CHECK(b[7] == true);
        CHECK(b[1] == false);
    }
    {
        // pos and n parameters
        std::string str("XX10110101XX");
        // pos=2, n=8 -> "10110101"
        std::bitset<8> b(str, 2, 8);
        CHECK(b.to_ulong() == 0xB5UL); // 10110101 = 0xB5
    }
    {
        // n larger than remaining — clamped
        std::string str("1010");
        std::bitset<8> b(str, 0, std::string::npos);
        CHECK(b[0] == false);
        CHECK(b[1] == true);
        CHECK(b[2] == false);
        CHECK(b[3] == true);
        for (size_t i = 4; i < 8; ++i) CHECK(!b[i]);
    }
    {
        // custom characters
        std::string str("TFTF");
        std::bitset<4> b(str, 0, std::string::npos, 'F', 'T');
        // "TFTF" with F=0 T=1: index 0=F=0, 1=T=1, 2=F=0, 3=T=1
        CHECK(!b[0]);
        CHECK(b[1]);
        CHECK(!b[2]);
        CHECK(b[3]);
    }
}

// ----------------------------------------------------------------
// set / reset / flip
// ----------------------------------------------------------------

void test_bitset_set_reset_flip() {
    {
        std::bitset<8> b;
        b.set();
        CHECK(b.all());
        CHECK(b.count() == 8);
    }
    {
        std::bitset<8> b;
        b.set(3);
        CHECK(b[3]);
        CHECK(b.count() == 1);
        b.set(3, false);
        CHECK(!b[3]);
        CHECK(b.none());
    }
    {
        std::bitset<8> b;
        b.set();
        b.reset();
        CHECK(b.none());
    }
    {
        std::bitset<8> b;
        b.set();
        b.reset(5);
        CHECK(!b[5]);
        CHECK(b.count() == 7);
    }
    {
        std::bitset<8> b(0xA5ULL); // 10100101
        b.flip();
        CHECK(b.to_ulong() == 0x5AUL); // 01011010
    }
    {
        std::bitset<8> b;
        b.flip(3);
        CHECK(b[3]);
        b.flip(3);
        CHECK(!b[3]);
    }
    {
        // chaining
        std::bitset<8> b;
        b.set().reset(0).flip(1);
        CHECK(!b[0]);
        CHECK(!b[1]); // was set (by set()), then flipped (back to 0)
        CHECK(b[2]);
    }
}

// ----------------------------------------------------------------
// reference proxy
// ----------------------------------------------------------------

void test_bitset_reference() {
    std::bitset<8> b;
    b[3] = true;
    CHECK(b[3]);
    CHECK(b.count() == 1);

    b[3] = false;
    CHECK(!b[3]);

    b[5] = true;
    CHECK(~b[3]); // ~false == true
    CHECK(!~b[5]); // ~true == false

    b[0] = b[5]; // copy from reference
    CHECK(b[0]);

    b[5].flip();
    CHECK(!b[5]);
}

// ----------------------------------------------------------------
// test() / []
// ----------------------------------------------------------------

void test_bitset_element_access() {
    std::bitset<8> b(0xA5ULL); // 10100101
    CHECK(b.test(0));
    CHECK(!b.test(1));
    CHECK(b.test(2));
    CHECK(!b.test(3));
    CHECK(!b.test(4));
    CHECK(b.test(5));
    CHECK(!b.test(6));
    CHECK(b.test(7));

    // const operator[]
    const std::bitset<8> cb(0xA5ULL);
    CHECK(cb[0]);
    CHECK(!cb[1]);
    CHECK(cb[7]);

    // constexpr
    constexpr std::bitset<8> ccb(0xA5ULL);
    static_assert(ccb[0]);
    static_assert(!ccb[1]);
    static_assert(ccb[7]);
}

// ----------------------------------------------------------------
// count / size / all / any / none
// ----------------------------------------------------------------

void test_bitset_observers() {
    {
        std::bitset<0> b;
        CHECK(b.size() == 0);
        CHECK(b.count() == 0);
        CHECK(b.none());
        CHECK(!b.any());
        CHECK(b.all()); // vacuously true
    }
    {
        std::bitset<8> b;
        CHECK(b.none());
        CHECK(!b.any());
        CHECK(!b.all());
        CHECK(b.count() == 0);
    }
    {
        std::bitset<8> b;
        b.set();
        CHECK(b.all());
        CHECK(b.any());
        CHECK(!b.none());
        CHECK(b.count() == 8);
    }
    {
        std::bitset<8> b(0x01ULL);
        CHECK(!b.none());
        CHECK(b.any());
        CHECK(!b.all());
        CHECK(b.count() == 1);
    }
    {
        // 65-bit bitset
        std::bitset<65> b;
        b.set();
        CHECK(b.all());
        CHECK(b.count() == 65);
        b.reset(64);
        CHECK(!b.all());
        CHECK(b.count() == 64);
    }
    {
        // count uses __builtin_popcountl, verify for multi-word
        std::bitset<128> b;
        b.set();
        CHECK(b.count() == 128);
        b.reset(0);
        b.reset(63);
        b.reset(64);
        b.reset(127);
        CHECK(b.count() == 124);
    }
}

// ----------------------------------------------------------------
// operator== / !=
// ----------------------------------------------------------------

void test_bitset_equality() {
    std::bitset<8> a(0xA5ULL);
    std::bitset<8> b(0xA5ULL);
    std::bitset<8> c(0x5AULL);

    CHECK(a == b);
    CHECK(!(a == c));
    // C++20: != synthesized from ==
    CHECK(a != c);

    std::bitset<0> e1, e2;
    CHECK(e1 == e2);
}

// ----------------------------------------------------------------
// shift operators
// ----------------------------------------------------------------

void test_bitset_shifts() {
    {
        std::bitset<8> b(0x01ULL);
        b <<= 3;
        CHECK(b.to_ulong() == 0x08UL);
    }
    {
        std::bitset<8> b(0x80ULL);
        b >>= 3;
        CHECK(b.to_ulong() == 0x10UL);
    }
    {
        std::bitset<8> b(0xFFULL);
        b <<= 8;
        CHECK(b.none());
    }
    {
        std::bitset<8> b(0xFFULL);
        b >>= 8;
        CHECK(b.none());
    }
    {
        std::bitset<8> b(0xFFULL);
        b <<= 0;
        CHECK(b.to_ulong() == 0xFFUL);
    }
    {
        // non-member
        std::bitset<8> b(0x01ULL);
        std::bitset<8> c = b << 4;
        CHECK(c.to_ulong() == 0x10UL);
        CHECK(b.to_ulong() == 0x01UL); // b unchanged
    }
    {
        std::bitset<8> b(0x80ULL);
        std::bitset<8> c = b >> 4;
        CHECK(c.to_ulong() == 0x08UL);
    }
    {
        // large shift, multi-word
        std::bitset<128> b;
        b.set(0);
        b <<= 65;
        CHECK(b.test(65));
        CHECK(!b.test(0));
        CHECK(b.count() == 1);
    }
    {
        std::bitset<128> b;
        b.set(65);
        b >>= 65;
        CHECK(b.test(0));
        CHECK(!b.test(65));
        CHECK(b.count() == 1);
    }
}

// ----------------------------------------------------------------
// bitwise operators (&, |, ^, ~)
// ----------------------------------------------------------------

void test_bitset_bitwise_ops() {
    std::bitset<8> a(0xF0ULL);
    std::bitset<8> b(0x0FULL);

    CHECK((a & b).none());
    CHECK((a | b).all());
    CHECK((a ^ b).all());
    CHECK((~a).to_ulong() == 0x0FUL);

    std::bitset<8> c(0xA5ULL);
    std::bitset<8> d(0x5AULL);
    CHECK((c & d).none());
    CHECK((c | d).all());
    CHECK((c ^ d).all());

    // &= |= ^=
    std::bitset<8> x(0xA5ULL);
    x &= std::bitset<8>(0xF0ULL);
    CHECK(x.to_ulong() == 0xA0UL);

    x |= std::bitset<8>(0x05ULL);
    CHECK(x.to_ulong() == 0xA5UL);

    x ^= std::bitset<8>(0xFFULL);
    CHECK(x.to_ulong() == 0x5AUL);
}

// ----------------------------------------------------------------
// to_ulong / to_ullong
// ----------------------------------------------------------------

void test_bitset_to_ulong_ullong() {
    {
        std::bitset<0> b;
        CHECK(b.to_ulong() == 0UL);
        CHECK(b.to_ullong() == 0ULL);
    }
    {
        std::bitset<8> b(0xA5ULL);
        CHECK(b.to_ulong() == 0xA5UL);
        CHECK(b.to_ullong() == 0xA5ULL);
    }
    {
        std::bitset<32> b(0xDEADBEEFULL);
        CHECK(b.to_ulong() == 0xDEADBEEFUL);
        CHECK(b.to_ullong() == 0xDEADBEEFULL);
    }
    {
        std::bitset<64> b(0xCAFEBABEDEADBEEFULL);
        CHECK(b.to_ullong() == 0xCAFEBABEDEADBEEFULL);
    }
}

// ----------------------------------------------------------------
// to_string — standard bitset string serialization via to_string()
// (to_string_buf is a libcis extension; the standard provides to_string)
// ----------------------------------------------------------------

void test_bitset_to_string_buf() {
    {
        std::bitset<8> b(0xA5ULL); // 10100101
        std::string s = b.to_string();
        CHECK(s.size() == 8);
        CHECK(s[0] == '1'); // bit 7
        CHECK(s[1] == '0'); // bit 6
        CHECK(s[2] == '1'); // bit 5
        CHECK(s[3] == '0'); // bit 4
        CHECK(s[4] == '0'); // bit 3
        CHECK(s[5] == '1'); // bit 2
        CHECK(s[6] == '0'); // bit 1
        CHECK(s[7] == '1'); // bit 0
    }
    {
        std::bitset<0> b;
        std::string s = b.to_string();
        CHECK(s.empty());
    }
    {
        // Custom zero/one characters
        std::bitset<4> b(0xAULL); // 1010
        std::string s = b.to_string('F', 'T');
        // "TFTF" (bit 3..0 = 1,0,1,0)
        CHECK(s[0] == 'T');
        CHECK(s[1] == 'F');
        CHECK(s[2] == 'T');
        CHECK(s[3] == 'F');
    }
}

// ----------------------------------------------------------------
// hash<bitset<N>>
// ----------------------------------------------------------------

void test_bitset_hash() {
    std::hash<std::bitset<8>> h;
    std::bitset<8> a(0xA5ULL);
    std::bitset<8> b(0xA5ULL);
    std::bitset<8> c(0x5AULL);

    CHECK(h(a) == h(b));
    // Different values should (almost always) hash differently
    // (not a hard requirement, but sanity check for non-trivial values)
    // We only assert same-value == same-hash
    CHECK(h(a) == h(a));

    // hash<bitset<0>>
    std::hash<std::bitset<0>> h0;
    std::bitset<0> e1, e2;
    CHECK(h0(e1) == h0(e2));

    // multi-word
    std::hash<std::bitset<128>> h128;
    std::bitset<128> m1, m2;
    m1.set(0); m1.set(64);
    m2.set(0); m2.set(64);
    CHECK(h128(m1) == h128(m2));
}

// ----------------------------------------------------------------
// edge cases: N=0, N=1, N=64, N=65, N=128
// ----------------------------------------------------------------

void test_bitset_edge_cases() {
    // N=0: size is 0, all operations are vacuous
    {
        std::bitset<0> b;
        CHECK(b.size() == 0);
        CHECK(b.none());
        CHECK(b.all());   // vacuous
        CHECK(!b.any());
        CHECK(b.count() == 0);
        CHECK(b.to_ulong() == 0UL);
        CHECK(b.to_ullong() == 0ULL);
        b.set(); b.reset(); b.flip();
        CHECK(b.none());
        std::bitset<0> c(0xFFULL);
        CHECK(b == c);
    }
    // N=1
    {
        std::bitset<1> b;
        b.set(0);
        CHECK(b.all());
        CHECK(b.to_ulong() == 1UL);
        b.flip();
        CHECK(b.none());
        b.flip(0);
        CHECK(b[0]);
    }
    // N=64
    {
        std::bitset<64> b;
        b.set();
        CHECK(b.all());
        CHECK(b.count() == 64);
        CHECK(b.to_ullong() == ~0ULL);
    }
    // N=65 straddles word boundary
    {
        std::bitset<65> b;
        b.set(64);
        CHECK(b.test(64));
        CHECK(b.count() == 1);
        b.flip();
        CHECK(b.count() == 64); // 65-1=64 bits set
        CHECK(!b.test(64));
    }
    // N=128
    {
        std::bitset<128> b;
        b.set();
        CHECK(b.all());
        CHECK(b.count() == 128);
        b.reset(63);
        b.reset(64);
        CHECK(b.count() == 126);
        CHECK(!b.test(63));
        CHECK(!b.test(64));
    }
}

// ----------------------------------------------------------------
// flip preserves padding zero bits (hash consistency)
// ----------------------------------------------------------------

void test_bitset_flip_padding() {
    // Ensure that after flip(), bits above N are still 0.
    // This matters for count() and hash() correctness.
    std::bitset<65> b;
    b.flip();
    CHECK(b.count() == 65);
    b.flip();
    CHECK(b.count() == 0);

    std::bitset<3> b3;
    b3.flip();
    CHECK(b3.count() == 3);
    CHECK(b3.all());
    b3.flip();
    CHECK(b3.none());
}

// ----------------------------------------------------------------
// shift edge cases
// ----------------------------------------------------------------

void test_bitset_shift_edge() {
    // Shift by 0 is identity
    {
        std::bitset<8> b(0xA5ULL);
        CHECK((b << 0).to_ulong() == 0xA5UL);
        CHECK((b >> 0).to_ulong() == 0xA5UL);
    }
    // Shift by >= N yields zero
    {
        std::bitset<8> b(0xFFULL);
        CHECK((b << 8).none());
        CHECK((b >> 8).none());
        CHECK((b << 100).none());
        CHECK((b >> 100).none());
    }
    // Round-trip: shift left then right
    {
        std::bitset<8> b(0x0FULL);
        std::bitset<8> c = (b << 3) >> 3;
        CHECK(c.to_ulong() == 0x0FUL);
    }
}
