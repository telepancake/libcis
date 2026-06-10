// test/compare.cpp — libcis tests for <compare>
// Consolidated from libc++'s test/std/language.support/cmp/
// Each test is a free function void test_compare_<case>() with no args.
// Static assertions for compile-time; CHECK() for runtime.

#include <compare>
#include "test.h"

//===----------------------------------------------------------------------===//
// partial_ordering — static members and values
//===----------------------------------------------------------------------===//

void test_compare_partial_ordering_static_members() {
    // Just access each static member to ensure it compiles and links.
    (void)std::partial_ordering::less;
    (void)std::partial_ordering::equivalent;
    (void)std::partial_ordering::greater;
    (void)std::partial_ordering::unordered;
}

void test_compare_partial_ordering_equality() {
    auto& eq    = std::partial_ordering::equivalent;
    auto& lt    = std::partial_ordering::less;
    auto& gt    = std::partial_ordering::greater;
    auto& unord = std::partial_ordering::unordered;

    // operator== with 0
    CHECK( (eq == 0));
    CHECK(!(lt == 0));
    CHECK(!(gt == 0));
    CHECK(!(unord == 0));

    // operator!= with 0
    CHECK(!(eq != 0));
    CHECK( (lt != 0));
    CHECK( (gt != 0));
    CHECK( (unord != 0));

    // symmetric: 0 == v
    CHECK( (0 == eq));
    CHECK(!(0 == lt));
    CHECK(!(0 == gt));
    CHECK(!(0 == unord));
}

void test_compare_partial_ordering_relational() {
    auto& eq    = std::partial_ordering::equivalent;
    auto& lt    = std::partial_ordering::less;
    auto& gt    = std::partial_ordering::greater;
    auto& unord = std::partial_ordering::unordered;

    // v < 0
    CHECK( (lt    < 0));
    CHECK(!(eq    < 0));
    CHECK(!(gt    < 0));
    CHECK(!(unord < 0));

    // v > 0
    CHECK( (gt    > 0));
    CHECK(!(eq    > 0));
    CHECK(!(lt    > 0));
    CHECK(!(unord > 0));

    // v <= 0
    CHECK( (lt    <= 0));
    CHECK( (eq    <= 0));
    CHECK(!(gt    <= 0));
    CHECK(!(unord <= 0));

    // v >= 0
    CHECK( (gt    >= 0));
    CHECK( (eq    >= 0));
    CHECK(!(lt    >= 0));
    CHECK(!(unord >= 0));

    // 0 < v  (i.e., v > 0 from the left)
    CHECK( (0 < gt));
    CHECK(!(0 < eq));
    CHECK(!(0 < lt));
    CHECK(!(0 < unord));

    // 0 > v
    CHECK( (0 > lt));
    CHECK(!(0 > eq));
    CHECK(!(0 > gt));
    CHECK(!(0 > unord));
}

void test_compare_partial_ordering_spaceship() {
    // partial_ordering <=> 0 returns the same value
    std::partial_ordering r1 = (std::partial_ordering::equivalent <=> 0);
    CHECK(r1 == 0);

    std::partial_ordering r2 = (std::partial_ordering::less <=> 0);
    CHECK(r2 < 0);

    std::partial_ordering r3 = (std::partial_ordering::greater <=> 0);
    CHECK(r3 > 0);

    std::partial_ordering r4 = (std::partial_ordering::unordered <=> 0);
    CHECK(r4 != 0);
    CHECK(!(r4 < 0));
    CHECK(!(r4 > 0));

    // 0 <=> v flips ordering
    std::partial_ordering r5 = (0 <=> std::partial_ordering::less);
    CHECK(r5 > 0);

    std::partial_ordering r6 = (0 <=> std::partial_ordering::greater);
    CHECK(r6 < 0);

    std::partial_ordering r7 = (0 <=> std::partial_ordering::equivalent);
    CHECK(r7 == 0);
}

void test_compare_partial_ordering_self_equality() {
    static_assert(std::partial_ordering::less == std::partial_ordering::less);
    static_assert(std::partial_ordering::equivalent == std::partial_ordering::equivalent);
    static_assert(std::partial_ordering::greater == std::partial_ordering::greater);
    static_assert(std::partial_ordering::unordered == std::partial_ordering::unordered);
    static_assert(std::partial_ordering::less != std::partial_ordering::equivalent);
    static_assert(std::partial_ordering::less != std::partial_ordering::greater);
    static_assert(std::partial_ordering::less != std::partial_ordering::unordered);
}

//===----------------------------------------------------------------------===//
// weak_ordering — static members and values
//===----------------------------------------------------------------------===//

void test_compare_weak_ordering_static_members() {
    (void)std::weak_ordering::less;
    (void)std::weak_ordering::equivalent;
    (void)std::weak_ordering::greater;
}

void test_compare_weak_ordering_equality() {
    auto& eq = std::weak_ordering::equivalent;
    auto& lt = std::weak_ordering::less;
    auto& gt = std::weak_ordering::greater;

    CHECK( (eq == 0));
    CHECK(!(lt == 0));
    CHECK(!(gt == 0));
    CHECK( (eq != 0) == false);
    CHECK( (lt != 0));
    CHECK( (gt != 0));
}

void test_compare_weak_ordering_relational() {
    auto& eq = std::weak_ordering::equivalent;
    auto& lt = std::weak_ordering::less;
    auto& gt = std::weak_ordering::greater;

    CHECK( (lt < 0)); CHECK(!(eq < 0)); CHECK(!(gt < 0));
    CHECK( (gt > 0)); CHECK(!(eq > 0)); CHECK(!(lt > 0));
    CHECK( (lt <= 0)); CHECK( (eq <= 0)); CHECK(!(gt <= 0));
    CHECK( (gt >= 0)); CHECK( (eq >= 0)); CHECK(!(lt >= 0));
    CHECK( (0 < gt)); CHECK(!(0 < eq)); CHECK(!(0 < lt));
    CHECK( (0 > lt)); CHECK(!(0 > eq)); CHECK(!(0 > gt));
}

void test_compare_weak_ordering_conversion() {
    // weak_ordering converts to partial_ordering
    std::partial_ordering wlt = std::weak_ordering::less;
    std::partial_ordering weq = std::weak_ordering::equivalent;
    std::partial_ordering wgt = std::weak_ordering::greater;

    CHECK(wlt == std::partial_ordering::less);
    CHECK(weq == std::partial_ordering::equivalent);
    CHECK(wgt == std::partial_ordering::greater);
}

//===----------------------------------------------------------------------===//
// strong_ordering — static members and values
//===----------------------------------------------------------------------===//

void test_compare_strong_ordering_static_members() {
    (void)std::strong_ordering::less;
    (void)std::strong_ordering::equal;
    (void)std::strong_ordering::equivalent;
    (void)std::strong_ordering::greater;
}

void test_compare_strong_ordering_equality() {
    auto& eq  = std::strong_ordering::equal;
    auto& seq = std::strong_ordering::equivalent;
    auto& lt  = std::strong_ordering::less;
    auto& gt  = std::strong_ordering::greater;

    CHECK( (eq == 0));
    CHECK( (seq == 0));
    CHECK(!(lt == 0));
    CHECK(!(gt == 0));
    CHECK( (lt != 0));
    CHECK( (gt != 0));
}

void test_compare_strong_ordering_relational() {
    auto& eq = std::strong_ordering::equal;
    auto& lt = std::strong_ordering::less;
    auto& gt = std::strong_ordering::greater;

    CHECK( (lt < 0)); CHECK(!(eq < 0)); CHECK(!(gt < 0));
    CHECK( (gt > 0)); CHECK(!(eq > 0)); CHECK(!(lt > 0));
    CHECK( (lt <= 0)); CHECK( (eq <= 0)); CHECK(!(gt <= 0));
    CHECK( (gt >= 0)); CHECK( (eq >= 0)); CHECK(!(lt >= 0));
}

void test_compare_strong_ordering_conversion() {
    // strong_ordering → partial_ordering
    std::partial_ordering slt = std::strong_ordering::less;
    std::partial_ordering seq = std::strong_ordering::equal;
    std::partial_ordering sgt = std::strong_ordering::greater;
    CHECK(slt == std::partial_ordering::less);
    CHECK(seq == std::partial_ordering::equivalent);
    CHECK(sgt == std::partial_ordering::greater);

    // strong_ordering → weak_ordering
    std::weak_ordering wlt = std::strong_ordering::less;
    std::weak_ordering weq = std::strong_ordering::equal;
    std::weak_ordering wgt = std::strong_ordering::greater;
    CHECK(wlt == std::weak_ordering::less);
    CHECK(weq == std::weak_ordering::equivalent);
    CHECK(wgt == std::weak_ordering::greater);
}

void test_compare_strong_ordering_equal_equivalent_same() {
    // equal and equivalent both compare == with 0
    CHECK( (std::strong_ordering::equal == 0));
    CHECK( (std::strong_ordering::equivalent == 0));
    // They're the same value
    static_assert(std::strong_ordering::equal == std::strong_ordering::equivalent);
}

//===----------------------------------------------------------------------===//
// named comparison functions
//===----------------------------------------------------------------------===//

void test_compare_named_functions() {
    using po = std::partial_ordering;

    CHECK( std::is_eq  (po::equivalent));
    CHECK(!std::is_eq  (po::less));
    CHECK(!std::is_eq  (po::greater));
    CHECK(!std::is_eq  (po::unordered));

    CHECK(!std::is_neq (po::equivalent));
    CHECK( std::is_neq (po::less));
    CHECK( std::is_neq (po::greater));
    CHECK( std::is_neq (po::unordered));

    CHECK(!std::is_lt  (po::equivalent));
    CHECK( std::is_lt  (po::less));
    CHECK(!std::is_lt  (po::greater));
    CHECK(!std::is_lt  (po::unordered));

    CHECK( std::is_lteq(po::equivalent));
    CHECK( std::is_lteq(po::less));
    CHECK(!std::is_lteq(po::greater));
    CHECK(!std::is_lteq(po::unordered));

    CHECK(!std::is_gt  (po::equivalent));
    CHECK(!std::is_gt  (po::less));
    CHECK( std::is_gt  (po::greater));
    CHECK(!std::is_gt  (po::unordered));

    CHECK( std::is_gteq(po::equivalent));
    CHECK(!std::is_gteq(po::less));
    CHECK( std::is_gteq(po::greater));
    CHECK(!std::is_gteq(po::unordered));
}

//===----------------------------------------------------------------------===//
// common_comparison_category
//===----------------------------------------------------------------------===//

void test_compare_common_comparison_category() {
    using PO = std::partial_ordering;
    using WO = std::weak_ordering;
    using SO = std::strong_ordering;

    // Empty pack -> strong_ordering
    static_assert(std::is_same_v<std::common_comparison_category_t<>, SO>);

    // Single types
    static_assert(std::is_same_v<std::common_comparison_category_t<SO>, SO>);
    static_assert(std::is_same_v<std::common_comparison_category_t<WO>, WO>);
    static_assert(std::is_same_v<std::common_comparison_category_t<PO>, PO>);

    // Non-category types -> void
    static_assert(std::is_same_v<std::common_comparison_category_t<void>, void>);
    static_assert(std::is_same_v<std::common_comparison_category_t<int*>, void>);
    static_assert(std::is_same_v<std::common_comparison_category_t<int>, void>);

    // partial_ordering dominates
    static_assert(std::is_same_v<std::common_comparison_category_t<SO, PO, SO>, PO>);
    static_assert(std::is_same_v<std::common_comparison_category_t<WO, PO, SO>, PO>);

    // weak_ordering when no partial
    static_assert(std::is_same_v<std::common_comparison_category_t<SO, WO, SO>, WO>);

    // strong stays strong
    static_assert(std::is_same_v<std::common_comparison_category_t<SO, SO>, SO>);

    // non-category in mix -> void
    static_assert(std::is_same_v<std::common_comparison_category_t<SO, void, SO>, void>);
}

//===----------------------------------------------------------------------===//
// compare_three_way_result
//===----------------------------------------------------------------------===//

// Helper: detect whether compare_three_way_result<T> has a ::type member.
// We can't use `requires { typename T::type; }` in gcc-10 (hard error instead of SFINAE).
// Use void_t / partial specialization instead.
template<class T, class = void>
inline constexpr bool has_ctr_type = false;
template<class T>
inline constexpr bool has_ctr_type<T, std::void_t<typename T::type>> = true;

void test_compare_three_way_result() {
    // int <=> int yields strong_ordering
    static_assert(std::is_same_v<std::compare_three_way_result_t<int>, std::strong_ordering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<int, int>, std::strong_ordering>);

    // float <=> float yields partial_ordering
    static_assert(std::is_same_v<std::compare_three_way_result_t<float>, std::partial_ordering>);
    static_assert(std::is_same_v<std::compare_three_way_result_t<double>, std::partial_ordering>);

    // Pointer types -> strong_ordering
    static_assert(std::is_same_v<std::compare_three_way_result_t<int*>, std::strong_ordering>);

    // Type without <=> should not have ::type
    struct NoSpaceship {};
    static_assert(!has_ctr_type<std::compare_three_way_result<NoSpaceship>>);
}

//===----------------------------------------------------------------------===//
// three_way_comparable concept
//===----------------------------------------------------------------------===//

void test_compare_three_way_comparable_concept() {
    // int is three_way_comparable with strong_ordering
    static_assert(std::three_way_comparable<int>);
    static_assert(std::three_way_comparable<int, std::strong_ordering>);

    // float is three_way_comparable with partial_ordering but NOT strong_ordering
    static_assert(std::three_way_comparable<float>);
    static_assert(std::three_way_comparable<float, std::partial_ordering>);
    static_assert(!std::three_way_comparable<float, std::strong_ordering>);

    // Pointer types
    static_assert(std::three_way_comparable<int*>);

    // Type without operator<=> should not satisfy the concept
    struct NoSpaceship {};
    static_assert(!std::three_way_comparable<NoSpaceship>);
}

void test_compare_three_way_comparable_with_concept() {
    static_assert(std::three_way_comparable_with<int, int>);
    // int <=> float is valid C++ (arithmetic promotions), so they ARE comparable_with
    static_assert(std::three_way_comparable_with<int, float>);

    // A type without == cannot satisfy the concept
    struct NoOps {};
    static_assert(!std::three_way_comparable_with<NoOps, NoOps>);
}

//===----------------------------------------------------------------------===//
// compare_three_way functor
//===----------------------------------------------------------------------===//

void test_compare_compare_three_way() {
    std::compare_three_way cmp;

    CHECK(cmp(1, 2) == std::strong_ordering::less);
    CHECK(cmp(2, 2) == std::strong_ordering::equal);
    CHECK(cmp(3, 2) == std::strong_ordering::greater);

    // Works with pointers
    int arr[3] = {1, 2, 3};
    CHECK(cmp(&arr[0], &arr[1]) == std::strong_ordering::less);
    CHECK(cmp(&arr[1], &arr[1]) == std::strong_ordering::equal);
    CHECK(cmp(&arr[2], &arr[1]) == std::strong_ordering::greater);

    // is_transparent present
    static_assert(std::is_same_v<std::compare_three_way::is_transparent, void>);
}

//===----------------------------------------------------------------------===//
// strong_order CPO
//===----------------------------------------------------------------------===//

void test_compare_strong_order_ints() {
    CHECK(std::strong_order(1, 2) == std::strong_ordering::less);
    CHECK(std::strong_order(2, 2) == std::strong_ordering::equal);
    CHECK(std::strong_order(3, 2) == std::strong_ordering::greater);
}

void test_compare_strong_order_floats() {
    // Basic ordering
    CHECK(std::strong_order(1.0f, 2.0f) == std::strong_ordering::less);
    CHECK(std::strong_order(2.0f, 2.0f) == std::strong_ordering::equal);
    CHECK(std::strong_order(3.0f, 2.0f) == std::strong_ordering::greater);

    CHECK(std::strong_order(1.0,  2.0)  == std::strong_ordering::less);
    CHECK(std::strong_order(2.0,  2.0)  == std::strong_ordering::equal);
    CHECK(std::strong_order(3.0,  2.0)  == std::strong_ordering::greater);

    // Negative zero < positive zero in total order
    CHECK(std::strong_order(-0.0f, 0.0f) == std::strong_ordering::less);
    CHECK(std::strong_order( 0.0f,-0.0f) == std::strong_ordering::greater);
    CHECK(std::strong_order(-0.0,  0.0)  == std::strong_ordering::less);
    CHECK(std::strong_order( 0.0, -0.0)  == std::strong_ordering::greater);

    // inf
    float inf = __builtin_inff();
    float ninf = -__builtin_inff();
    CHECK(std::strong_order(ninf, -1.0f) == std::strong_ordering::less);
    CHECK(std::strong_order(1.0f, inf)   == std::strong_ordering::less);
    CHECK(std::strong_order(inf, inf)    == std::strong_ordering::equal);
}

void test_compare_strong_order_floats_nan() {
    float pnan = __builtin_nanf("");
    float nnan = __builtin_copysignf(__builtin_nanf(""), -1.0f);

    // NaN compares are only valid at runtime (not constexpr)
    // negative NaN < positive NaN in total order
    CHECK(std::strong_order(nnan, pnan) == std::strong_ordering::less);
    CHECK(std::strong_order(pnan, nnan) == std::strong_ordering::greater);
    CHECK(std::strong_order(nnan, nnan) == std::strong_ordering::equal);
    CHECK(std::strong_order(pnan, pnan) == std::strong_ordering::equal);

    // NaN > any finite number (positive NaN)
    CHECK(std::strong_order(1.0f, pnan)  == std::strong_ordering::less);
    CHECK(std::strong_order(pnan, 1.0f)  == std::strong_ordering::greater);

    // Negative NaN < any finite number
    CHECK(std::strong_order(nnan, 1.0f)  == std::strong_ordering::less);
    CHECK(std::strong_order(1.0f, nnan)  == std::strong_ordering::greater);
}

namespace test_strong_order_adl_ns {
    struct A {
        // A has operator<=> returning strong_ordering so that compare_three_way works,
        // but we also provide an ADL strong_order that returns a different value
        // to distinguish which path is taken.
        bool operator==(const A&) const = default;
        std::strong_ordering operator<=>(const A&) const { return std::strong_ordering::equal; }
    };
    // ADL strong_order returns "less" — compare_three_way would return "equal"
    std::strong_ordering strong_order(const A&, const A&) {
        return std::strong_ordering::less;
    }
}

void test_compare_strong_order_adl() {
    test_strong_order_adl_ns::A a;
    // ADL finds strong_order in test_strong_order_adl_ns which returns less (priority > operator<=>)
    CHECK(std::strong_order(a, a) == std::strong_ordering::less);
}

//===----------------------------------------------------------------------===//
// weak_order CPO
//===----------------------------------------------------------------------===//

void test_compare_weak_order_ints() {
    CHECK(std::weak_order(1, 2) == std::weak_ordering::less);
    CHECK(std::weak_order(2, 2) == std::weak_ordering::equivalent);
    CHECK(std::weak_order(3, 2) == std::weak_ordering::greater);
}

void test_compare_weak_order_floats() {
    CHECK(std::weak_order(1.0, 2.0) == std::weak_ordering::less);
    CHECK(std::weak_order(2.0, 2.0) == std::weak_ordering::equivalent);
    CHECK(std::weak_order(3.0, 2.0) == std::weak_ordering::greater);

    // All NaN are equivalent in weak_order
    float pnan = __builtin_nanf("");
    float nnan = __builtin_copysignf(__builtin_nanf(""), -1.0f);
    // Both NaN, negative < positive
    CHECK(std::weak_order(nnan, pnan) == std::weak_ordering::less);
    CHECK(std::weak_order(pnan, nnan) == std::weak_ordering::greater);
}

//===----------------------------------------------------------------------===//
// partial_order CPO
//===----------------------------------------------------------------------===//

void test_compare_partial_order_ints() {
    CHECK(std::partial_order(1, 2) == std::partial_ordering::less);
    CHECK(std::partial_order(2, 2) == std::partial_ordering::equivalent);
    CHECK(std::partial_order(3, 2) == std::partial_ordering::greater);
}

void test_compare_partial_order_floats() {
    CHECK(std::partial_order(1.0, 2.0) == std::partial_ordering::less);
    CHECK(std::partial_order(2.0, 2.0) == std::partial_ordering::equivalent);
    CHECK(std::partial_order(3.0, 2.0) == std::partial_ordering::greater);

    float pnan = __builtin_nanf("");
    // NaN <=> NaN is unordered in partial_order
    CHECK(std::partial_order(pnan, pnan) == std::partial_ordering::unordered);
    CHECK(std::partial_order(pnan, 1.0f) == std::partial_ordering::unordered);
    CHECK(std::partial_order(1.0f, pnan) == std::partial_ordering::unordered);
}

//===----------------------------------------------------------------------===//
// compare_strong_order_fallback CPO
//===----------------------------------------------------------------------===//

void test_compare_compare_strong_order_fallback_ints() {
    CHECK(std::compare_strong_order_fallback(1, 2) == std::strong_ordering::less);
    CHECK(std::compare_strong_order_fallback(2, 2) == std::strong_ordering::equal);
    CHECK(std::compare_strong_order_fallback(3, 2) == std::strong_ordering::greater);
}

// Type with only == and < (no <=>): uses fallback path
namespace test_fallback_ns {
    struct OldStyle {
        int v;
        bool operator==(const OldStyle& o) const { return v == o.v; }
        bool operator< (const OldStyle& o) const { return v <  o.v; }
    };
}

void test_compare_compare_strong_order_fallback_oldstyle() {
    using test_fallback_ns::OldStyle;
    OldStyle a{1}, b{2}, c{2};
    CHECK(std::compare_strong_order_fallback(a, b) == std::strong_ordering::less);
    CHECK(std::compare_strong_order_fallback(b, c) == std::strong_ordering::equal);
    CHECK(std::compare_strong_order_fallback(b, a) == std::strong_ordering::greater);
}

//===----------------------------------------------------------------------===//
// compare_weak_order_fallback CPO
//===----------------------------------------------------------------------===//

void test_compare_compare_weak_order_fallback_ints() {
    CHECK(std::compare_weak_order_fallback(1, 2) == std::weak_ordering::less);
    CHECK(std::compare_weak_order_fallback(2, 2) == std::weak_ordering::equivalent);
    CHECK(std::compare_weak_order_fallback(3, 2) == std::weak_ordering::greater);
}

void test_compare_compare_weak_order_fallback_oldstyle() {
    using test_fallback_ns::OldStyle;
    OldStyle a{1}, b{2}, c{2};
    CHECK(std::compare_weak_order_fallback(a, b) == std::weak_ordering::less);
    CHECK(std::compare_weak_order_fallback(b, c) == std::weak_ordering::equivalent);
    CHECK(std::compare_weak_order_fallback(b, a) == std::weak_ordering::greater);
}

//===----------------------------------------------------------------------===//
// compare_partial_order_fallback CPO
//===----------------------------------------------------------------------===//

void test_compare_compare_partial_order_fallback_ints() {
    CHECK(std::compare_partial_order_fallback(1, 2) == std::partial_ordering::less);
    CHECK(std::compare_partial_order_fallback(2, 2) == std::partial_ordering::equivalent);
    CHECK(std::compare_partial_order_fallback(3, 2) == std::partial_ordering::greater);
}

// Type that can have unordered result (simulated float-like)
namespace test_partial_fallback_ns {
    struct Fuzzy {
        float v;
        bool operator==(const Fuzzy& o) const { return v == o.v; }
        bool operator< (const Fuzzy& o) const { return v <  o.v; }
        // No >, >=, <=
    };
    // Make it work: we need all comparisons for partial_ordered_with,
    // but for the fallback (priority_tag<0>), only ==, <, and reversed < are needed.
}

void test_compare_compare_partial_order_fallback_unordered() {
    // Use float directly: partial_order gives unordered for NaN
    float pnan = __builtin_nanf("");
    CHECK(std::compare_partial_order_fallback(pnan, pnan) == std::partial_ordering::unordered);
    CHECK(std::compare_partial_order_fallback(pnan, 1.0f) == std::partial_ordering::unordered);
    CHECK(std::compare_partial_order_fallback(1.0f, pnan) == std::partial_ordering::unordered);
}

//===----------------------------------------------------------------------===//
// synth_three_way — exposition-only; not a public std:: name.
// Replaced by direct <=> tests and compare_weak_order_fallback for the
// "old-style" case (std::synth_three_way is not in the standard's public API).
//===----------------------------------------------------------------------===//

void test_compare_synth_three_way() {
    // For types that support <=>, use operator<=> directly.
    // The standard does not provide std::synth_three_way as a public name.
    auto r1 = (1 <=> 2);
    CHECK(r1 == std::strong_ordering::less);
    auto r2 = (2 <=> 2);
    CHECK(r2 == std::strong_ordering::equal);
    auto r3 = (3 <=> 2);
    CHECK(r3 == std::strong_ordering::greater);
    // int <=> int yields strong_ordering; it converts to weak_ordering
    std::weak_ordering wl = (1 <=> 2);
    CHECK(wl == std::weak_ordering::less);
    std::weak_ordering we = (2 <=> 2);
    CHECK(we == std::weak_ordering::equivalent);
    std::weak_ordering wg = (3 <=> 2);
    CHECK(wg == std::weak_ordering::greater);
}

namespace synth_old_ns {
    struct OldStyle {
        int v;
        bool operator==(const OldStyle&) const = default;
        bool operator< (const OldStyle& o) const { return v < o.v; }
    };
}

void test_compare_synth_three_way_fallback() {
    // For a type without <=>, compare_weak_order_fallback synthesises ordering
    // from == and < (the same logic std::synth_three_way used internally).
    using synth_old_ns::OldStyle;
    OldStyle a{1}, b{2}, c{2};
    auto ra = std::compare_weak_order_fallback(a, b);
    CHECK(ra == std::weak_ordering::less);
    auto rb = std::compare_weak_order_fallback(b, c);
    CHECK(rb == std::weak_ordering::equivalent);
    auto rc = std::compare_weak_order_fallback(b, a);
    CHECK(rc == std::weak_ordering::greater);
}

//===----------------------------------------------------------------------===//
// Compile-time constexpr correctness checks
//===----------------------------------------------------------------------===//

void test_compare_constexpr_ordering() {
    // All the category values and comparisons should be constexpr
    static_assert(std::partial_ordering::less    < 0);
    static_assert(std::partial_ordering::greater > 0);
    static_assert(std::partial_ordering::equivalent == 0);
    static_assert(std::partial_ordering::unordered != 0);

    static_assert(std::weak_ordering::less    < 0);
    static_assert(std::weak_ordering::greater > 0);
    static_assert(std::weak_ordering::equivalent == 0);

    static_assert(std::strong_ordering::less    < 0);
    static_assert(std::strong_ordering::greater > 0);
    static_assert(std::strong_ordering::equal == 0);
    static_assert(std::strong_ordering::equivalent == 0);

    // Conversions are constexpr
    constexpr std::partial_ordering p1 = std::strong_ordering::less;
    static_assert(p1 < 0);
    constexpr std::weak_ordering w1 = std::strong_ordering::greater;
    static_assert(w1 > 0);
}

void test_compare_is_eq_constexpr() {
    static_assert(std::is_eq  (std::partial_ordering::equivalent));
    static_assert(!std::is_eq  (std::partial_ordering::less));
    static_assert(std::is_lt  (std::partial_ordering::less));
    static_assert(std::is_gt  (std::partial_ordering::greater));
    static_assert(std::is_lteq(std::partial_ordering::less));
    static_assert(std::is_lteq(std::partial_ordering::equivalent));
    static_assert(std::is_gteq(std::partial_ordering::greater));
    static_assert(std::is_gteq(std::partial_ordering::equivalent));
}
