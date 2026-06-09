// Tests for <ratio>
// Ported from libc++ test/std/utilities/ratio/
#include <ratio>
#include "test.h"

//===----------------------------------------------------------------------===//
// ratio basic: num and den are reduced with sign on num
//===----------------------------------------------------------------------===//
void test_ratio_basic() {
    static_assert(std::ratio<1, 1>::num == 1 && std::ratio<1, 1>::den == 1);
    static_assert(std::ratio<1, 10>::num == 1 && std::ratio<1, 10>::den == 10);
    static_assert(std::ratio<10, 10>::num == 1 && std::ratio<10, 10>::den == 1);
    static_assert(std::ratio<10, 1>::num == 10 && std::ratio<10, 1>::den == 1);
    static_assert(std::ratio<12, 4>::num == 3 && std::ratio<12, 4>::den == 1);
    static_assert(std::ratio<12, -4>::num == -3 && std::ratio<12, -4>::den == 1);
    static_assert(std::ratio<-12, 4>::num == -3 && std::ratio<-12, 4>::den == 1);
    static_assert(std::ratio<-12, -4>::num == 3 && std::ratio<-12, -4>::den == 1);
    static_assert(std::ratio<4, 12>::num == 1 && std::ratio<4, 12>::den == 3);
    static_assert(std::ratio<4, -12>::num == -1 && std::ratio<4, -12>::den == 3);
    static_assert(std::ratio<-4, 12>::num == -1 && std::ratio<-4, 12>::den == 3);
    static_assert(std::ratio<-4, -12>::num == 1 && std::ratio<-4, -12>::den == 3);
    static_assert(std::ratio<222, 333>::num == 2 && std::ratio<222, 333>::den == 3);
    static_assert(std::ratio<222, -333>::num == -2 && std::ratio<222, -333>::den == 3);
    static_assert(std::ratio<0x7FFFFFFFFFFFFFFFLL, 127>::num == 72624976668147841LL
               && std::ratio<0x7FFFFFFFFFFFFFFFLL, 127>::den == 1);
    // type member is ratio<num,den>
    static_assert(std::ratio<6, 4>::type::num == 3 && std::ratio<6, 4>::type::den == 2);
}

//===----------------------------------------------------------------------===//
// ratio_add
//===----------------------------------------------------------------------===//
void test_ratio_add() {
    // 1/1 + 1/1 = 2/1
    using R1 = std::ratio_add<std::ratio<1,1>, std::ratio<1,1>>;
    static_assert(R1::num == 2 && R1::den == 1);

    // 1/2 + 1/1 = 3/2
    using R2 = std::ratio_add<std::ratio<1,2>, std::ratio<1,1>>;
    static_assert(R2::num == 3 && R2::den == 2);

    // -1/2 + 1/1 = 1/2
    using R3 = std::ratio_add<std::ratio<-1,2>, std::ratio<1,1>>;
    static_assert(R3::num == 1 && R3::den == 2);

    // 1/-2 + 1/1 = 1/2
    using R4 = std::ratio_add<std::ratio<1,-2>, std::ratio<1,1>>;
    static_assert(R4::num == 1 && R4::den == 2);

    // 1/2 + -1/1 = -1/2
    using R5 = std::ratio_add<std::ratio<1,2>, std::ratio<-1,1>>;
    static_assert(R5::num == -1 && R5::den == 2);

    // 1/2 + 1/-1 = -1/2
    using R6 = std::ratio_add<std::ratio<1,2>, std::ratio<1,-1>>;
    static_assert(R6::num == -1 && R6::den == 2);

    // 0 + anything = anything
    using R7 = std::ratio_add<std::ratio<0>, std::ratio<1,1>>;
    static_assert(R7::num == 1 && R7::den == 1);

    // large: 56987354/467584654 + 544668/22145
    // expected: 127970191639601 / 5177331081415
    using R8 = std::ratio_add<std::ratio<56987354, 467584654>, std::ratio<544668, 22145>>;
    static_assert(R8::num == 127970191639601LL && R8::den == 5177331081415LL);
}

//===----------------------------------------------------------------------===//
// ratio_subtract
//===----------------------------------------------------------------------===//
void test_ratio_subtract() {
    // 1/1 - 1/1 = 0/1
    using R1 = std::ratio_subtract<std::ratio<1,1>, std::ratio<1,1>>;
    static_assert(R1::num == 0 && R1::den == 1);

    // 1/2 - 1/1 = -1/2
    using R2 = std::ratio_subtract<std::ratio<1,2>, std::ratio<1,1>>;
    static_assert(R2::num == -1 && R2::den == 2);

    // -1/2 - 1/1 = -3/2
    using R3 = std::ratio_subtract<std::ratio<-1,2>, std::ratio<1,1>>;
    static_assert(R3::num == -3 && R3::den == 2);

    // 1/-2 - 1/1 = -3/2
    using R4 = std::ratio_subtract<std::ratio<1,-2>, std::ratio<1,1>>;
    static_assert(R4::num == -3 && R4::den == 2);

    // 1/2 - -1/1 = 3/2
    using R5 = std::ratio_subtract<std::ratio<1,2>, std::ratio<-1,1>>;
    static_assert(R5::num == 3 && R5::den == 2);

    // 1/2 - 1/-1 = 3/2
    using R6 = std::ratio_subtract<std::ratio<1,2>, std::ratio<1,-1>>;
    static_assert(R6::num == 3 && R6::den == 2);

    // large: 56987354/467584654 - 544668/22145
    // expected: -126708206685271 / 5177331081415
    using R7 = std::ratio_subtract<std::ratio<56987354, 467584654>, std::ratio<544668, 22145>>;
    static_assert(R7::num == -126708206685271LL && R7::den == 5177331081415LL);
}

//===----------------------------------------------------------------------===//
// ratio_multiply
//===----------------------------------------------------------------------===//
void test_ratio_multiply() {
    // 1/1 * 1/1 = 1/1
    using R1 = std::ratio_multiply<std::ratio<1,1>, std::ratio<1,1>>;
    static_assert(R1::num == 1 && R1::den == 1);

    // 1/2 * 2/3 = 1/3
    using R2 = std::ratio_multiply<std::ratio<1,2>, std::ratio<2,3>>;
    static_assert(R2::num == 1 && R2::den == 3);

    // 4/6 * 6/4 = 1/1
    using R3 = std::ratio_multiply<std::ratio<4,6>, std::ratio<6,4>>;
    static_assert(R3::num == 1 && R3::den == 1);

    // -1/2 * 2/3 = -1/3
    using R4 = std::ratio_multiply<std::ratio<-1,2>, std::ratio<2,3>>;
    static_assert(R4::num == -1 && R4::den == 3);

    // 1/-2 * 2/3 = -1/3
    using R5 = std::ratio_multiply<std::ratio<1,-2>, std::ratio<2,3>>;
    static_assert(R5::num == -1 && R5::den == 3);

    // 1/2 * -2/3 = -1/3
    using R6 = std::ratio_multiply<std::ratio<1,2>, std::ratio<-2,3>>;
    static_assert(R6::num == -1 && R6::den == 3);

    // 1/2 * 2/-3 = -1/3
    using R7 = std::ratio_multiply<std::ratio<1,2>, std::ratio<2,-3>>;
    static_assert(R7::num == -1 && R7::den == 3);

    // 2/3 * 3/2 = 1/1
    using R8 = std::ratio_multiply<std::ratio<2,3>, std::ratio<3,2>>;
    static_assert(R8::num == 1 && R8::den == 1);
}

//===----------------------------------------------------------------------===//
// ratio_divide
//===----------------------------------------------------------------------===//
void test_ratio_divide() {
    // 1/1 / 1/1 = 1/1
    using R1 = std::ratio_divide<std::ratio<1,1>, std::ratio<1,1>>;
    static_assert(R1::num == 1 && R1::den == 1);

    // 1/2 / 2/3 = 3/4
    using R2 = std::ratio_divide<std::ratio<1,2>, std::ratio<2,3>>;
    static_assert(R2::num == 3 && R2::den == 4);

    // -1/2 / 2/3 = -3/4
    using R3 = std::ratio_divide<std::ratio<-1,2>, std::ratio<2,3>>;
    static_assert(R3::num == -3 && R3::den == 4);

    // 1/-2 / 2/3 = -3/4
    using R4 = std::ratio_divide<std::ratio<1,-2>, std::ratio<2,3>>;
    static_assert(R4::num == -3 && R4::den == 4);

    // 1/2 / -2/3 = -3/4
    using R5 = std::ratio_divide<std::ratio<1,2>, std::ratio<-2,3>>;
    static_assert(R5::num == -3 && R5::den == 4);

    // 1/2 / 2/-3 = -3/4
    using R6 = std::ratio_divide<std::ratio<1,2>, std::ratio<2,-3>>;
    static_assert(R6::num == -3 && R6::den == 4);

    // 4/6 / 4/6 = 1/1
    using R7 = std::ratio_divide<std::ratio<4,6>, std::ratio<4,6>>;
    static_assert(R7::num == 1 && R7::den == 1);
}

//===----------------------------------------------------------------------===//
// ratio_equal / ratio_not_equal
//===----------------------------------------------------------------------===//
void test_ratio_equal() {
    static_assert(std::ratio_equal<std::ratio<1,1>, std::ratio<1,1>>::value);
    static_assert(!std::ratio_equal<std::ratio<1,1>, std::ratio<1,2>>::value);
    static_assert(std::ratio_equal_v<std::ratio<2,4>, std::ratio<1,2>>);
    static_assert(!std::ratio_equal_v<std::ratio<1,2>, std::ratio<1,1>>);

    static_assert(std::ratio_not_equal<std::ratio<1,1>, std::ratio<1,2>>::value);
    static_assert(!std::ratio_not_equal<std::ratio<1,1>, std::ratio<1,1>>::value);
    static_assert(std::ratio_not_equal_v<std::ratio<1,2>, std::ratio<1,1>>);
    static_assert(!std::ratio_not_equal_v<std::ratio<2,4>, std::ratio<1,2>>);
}

//===----------------------------------------------------------------------===//
// ratio_less / ratio_less_equal / ratio_greater / ratio_greater_equal
//===----------------------------------------------------------------------===//
void test_ratio_less() {
    // equal → not less
    static_assert(!std::ratio_less<std::ratio<1,1>, std::ratio<1,1>>::value);
    static_assert(!std::ratio_less_v<std::ratio<0x7FFFFFFFFFFFFFFFLL,1>, std::ratio<0x7FFFFFFFFFFFFFFFLL,1>>);
    static_assert(!std::ratio_less_v<std::ratio<-0x7FFFFFFFFFFFFFFFLL,1>, std::ratio<-0x7FFFFFFFFFFFFFFFLL,1>>);

    // positive < negative? No
    static_assert(!std::ratio_less_v<std::ratio<1,1>, std::ratio<1,-1>>);
    // positive < larger positive
    static_assert(std::ratio_less_v<std::ratio<1,2>, std::ratio<1,1>>);
    // negative < positive
    static_assert(std::ratio_less_v<std::ratio<-1,1>, std::ratio<1,1>>);
    static_assert(std::ratio_less_v<std::ratio<-0x7FFFFFFFFFFFFFFFLL,1>, std::ratio<0x7FFFFFFFFFFFFFFFLL,1>>);
    // 1/2 < 1/1
    static_assert(std::ratio_less<std::ratio<1,2>, std::ratio<1,1>>::value);
    // not less (reversed)
    static_assert(!std::ratio_less<std::ratio<1,1>, std::ratio<1,2>>::value);
}

void test_ratio_less_equal() {
    static_assert(std::ratio_less_equal<std::ratio<1,1>, std::ratio<1,1>>::value);
    static_assert(std::ratio_less_equal_v<std::ratio<1,2>, std::ratio<1,1>>);
    static_assert(!std::ratio_less_equal_v<std::ratio<1,1>, std::ratio<1,2>>);
}

void test_ratio_greater() {
    static_assert(!std::ratio_greater<std::ratio<1,1>, std::ratio<1,1>>::value);
    static_assert(std::ratio_greater_v<std::ratio<1,1>, std::ratio<1,2>>);
    static_assert(!std::ratio_greater_v<std::ratio<1,2>, std::ratio<1,1>>);
}

void test_ratio_greater_equal() {
    static_assert(std::ratio_greater_equal<std::ratio<1,1>, std::ratio<1,1>>::value);
    static_assert(std::ratio_greater_equal_v<std::ratio<1,1>, std::ratio<1,2>>);
    static_assert(!std::ratio_greater_equal_v<std::ratio<1,2>, std::ratio<1,1>>);
}

//===----------------------------------------------------------------------===//
// SI typedefs
//===----------------------------------------------------------------------===//
void test_ratio_si() {
    static_assert(std::atto::num  == 1 && std::atto::den  == 1000000000000000000LL);
    static_assert(std::femto::num == 1 && std::femto::den == 1000000000000000LL);
    static_assert(std::pico::num  == 1 && std::pico::den  == 1000000000000LL);
    static_assert(std::nano::num  == 1 && std::nano::den  == 1000000000LL);
    static_assert(std::micro::num == 1 && std::micro::den == 1000000LL);
    static_assert(std::milli::num == 1 && std::milli::den == 1000LL);
    static_assert(std::centi::num == 1 && std::centi::den == 100LL);
    static_assert(std::deci::num  == 1 && std::deci::den  == 10LL);
    static_assert(std::deca::num  == 10LL  && std::deca::den  == 1);
    static_assert(std::hecto::num == 100LL && std::hecto::den == 1);
    static_assert(std::kilo::num  == 1000LL && std::kilo::den == 1);
    static_assert(std::mega::num  == 1000000LL && std::mega::den == 1);
    static_assert(std::giga::num  == 1000000000LL && std::giga::den == 1);
    static_assert(std::tera::num  == 1000000000000LL && std::tera::den == 1);
    static_assert(std::peta::num  == 1000000000000000LL && std::peta::den == 1);
    static_assert(std::exa::num   == 1000000000000000000LL && std::exa::den == 1);
}
