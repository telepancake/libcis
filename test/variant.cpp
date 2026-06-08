// Tests for <variant>.
// Each test is a free function void test_variant_<case>() with no args.
// Use CHECK(expr) for runtime assertions, static_assert for compile-time.
// No progress printing; failure traps.
#include <variant>
#include "test.h"

// ============================================================================
// test_variant_basic
// ============================================================================
void test_variant_basic() {
    // Default construction — uses first alternative
    std::variant<int, double> v;
    CHECK(v.index() == 0);
    CHECK(!v.valueless_by_exception());
    CHECK(std::get<int>(v) == 0);

    // Construction from value
    std::variant<int, double> v2(3.14);
    CHECK(v2.index() == 1);
    CHECK(std::get<double>(v2) == 3.14);

    std::variant<int, double> v3(42);
    CHECK(v3.index() == 0);
    CHECK(std::get<int>(v3) == 42);
}

// ============================================================================
// test_variant_size_alternative
// ============================================================================
void test_variant_size_alternative() {
    static_assert(std::variant_size_v<std::variant<int, double, char>> == 3);
    static_assert(std::variant_size_v<std::variant<int>> == 1);
    static_assert(std::variant_size_v<const std::variant<int, double>> == 2);

    static_assert(std::is_same_v<
        std::variant_alternative_t<0, std::variant<int, double, char>>, int>);
    static_assert(std::is_same_v<
        std::variant_alternative_t<1, std::variant<int, double, char>>, double>);
    static_assert(std::is_same_v<
        std::variant_alternative_t<2, std::variant<int, double, char>>, char>);

    // cv qualifications
    static_assert(std::is_same_v<
        std::variant_alternative_t<0, const std::variant<int>>, const int>);
}

// ============================================================================
// test_variant_holds_alternative
// ============================================================================
void test_variant_holds_alternative() {
    std::variant<int, double, char> v = 42;
    CHECK(std::holds_alternative<int>(v));
    CHECK(!std::holds_alternative<double>(v));
    CHECK(!std::holds_alternative<char>(v));

    v = 3.14;
    CHECK(!std::holds_alternative<int>(v));
    CHECK(std::holds_alternative<double>(v));
}

// ============================================================================
// test_variant_get
// ============================================================================
void test_variant_get() {
    std::variant<int, double, char> v(std::in_place_index<1>, 2.71828);
    CHECK(std::get<1>(v) == 2.71828);
    CHECK(std::get<double>(v) == 2.71828);

    // get_if
    CHECK(std::get_if<0>(&v) == nullptr);
    CHECK(std::get_if<1>(&v) != nullptr);
    CHECK(*std::get_if<1>(&v) == 2.71828);
    CHECK(std::get_if<double>(&v) != nullptr);
    CHECK(std::get_if<int>(&v) == nullptr);

    // const get
    const auto& cv = v;
    CHECK(std::get<double>(cv) == 2.71828);
    CHECK(std::get<1>(cv) == 2.71828);
}

// ============================================================================
// test_variant_inplace_construction
// ============================================================================
void test_variant_inplace_construction() {
    // in_place_index
    std::variant<int, double> v1(std::in_place_index<0>, 99);
    CHECK(v1.index() == 0);
    CHECK(std::get<0>(v1) == 99);

    std::variant<int, double> v2(std::in_place_index<1>, 1.5);
    CHECK(v2.index() == 1);
    CHECK(std::get<1>(v2) == 1.5);

    // in_place_type
    std::variant<int, double> v3(std::in_place_type<int>, 77);
    CHECK(v3.index() == 0);
    CHECK(std::get<int>(v3) == 77);
}

// ============================================================================
// test_variant_emplace
// ============================================================================
void test_variant_emplace() {
    std::variant<int, double, char> v;
    v.emplace<1>(3.14);
    CHECK(v.index() == 1);
    CHECK(std::get<1>(v) == 3.14);

    v.emplace<char>('x');
    CHECK(v.index() == 2);
    CHECK(std::get<char>(v) == 'x');

    v.emplace<0>(100);
    CHECK(v.index() == 0);
    CHECK(std::get<0>(v) == 100);
}

// ============================================================================
// test_variant_copy_move
// ============================================================================
void test_variant_copy_move() {
    std::variant<int, double> v1(42);
    std::variant<int, double> v2 = v1;
    CHECK(v2.index() == 0);
    CHECK(std::get<int>(v2) == 42);

    std::variant<int, double> v3 = std::move(v1);
    CHECK(v3.index() == 0);
    CHECK(std::get<int>(v3) == 42);
}

// ============================================================================
// test_variant_assignment
// ============================================================================
void test_variant_assignment() {
    std::variant<int, double> v(42);
    v = 3.14;
    CHECK(v.index() == 1);
    CHECK(std::get<double>(v) == 3.14);

    v = 99;
    CHECK(v.index() == 0);
    CHECK(std::get<int>(v) == 99);

    // Copy assignment from same type
    std::variant<int, double> v2(1.23);
    v = v2;
    CHECK(v.index() == 1);
    CHECK(std::get<double>(v) == 1.23);
}

// ============================================================================
// test_variant_swap
// ============================================================================
void test_variant_swap() {
    std::variant<int, double> v1(10);
    std::variant<int, double> v2(3.14);
    v1.swap(v2);
    CHECK(v1.index() == 1);
    CHECK(std::get<double>(v1) == 3.14);
    CHECK(v2.index() == 0);
    CHECK(std::get<int>(v2) == 10);

    // swap same index
    std::variant<int, double> v3(5);
    std::variant<int, double> v4(7);
    v3.swap(v4);
    CHECK(std::get<int>(v3) == 7);
    CHECK(std::get<int>(v4) == 5);
}

// ============================================================================
// test_variant_visit
// ============================================================================
void test_variant_visit() {
    std::variant<int, double, char> v(42);

    auto result = std::visit([](auto x) -> int {
        return static_cast<int>(x);
    }, v);
    CHECK(result == 42);

    v = 3.14;
    auto result2 = std::visit([](auto x) -> double {
        return static_cast<double>(x);
    }, v);
    CHECK(result2 == 3.14);

    // Multi-variant visit
    std::variant<int, double> va(3);
    std::variant<int, double> vb(4);
    auto sum = std::visit([](auto a, auto b) -> int {
        return static_cast<int>(a) + static_cast<int>(b);
    }, va, vb);
    CHECK(sum == 7);
}

// ============================================================================
// test_variant_visit_r
// ============================================================================
void test_variant_visit_r() {
    std::variant<int, double> v(42);
    double r = std::visit<double>([](auto x) { return static_cast<double>(x) * 2.0; }, v);
    CHECK(r == 84.0);
}

// ============================================================================
// test_variant_comparison
// ============================================================================
void test_variant_comparison() {
    std::variant<int, double> v1(1);
    std::variant<int, double> v2(1);
    std::variant<int, double> v3(2);
    std::variant<int, double> v4(1.0);  // index=1

    CHECK(v1 == v2);
    CHECK(!(v1 == v3));
    CHECK(v1 != v3);
    CHECK(v1 < v3);
    CHECK(v3 > v1);
    CHECK(v1 <= v2);
    CHECK(v1 >= v2);

    // Different index: index 0 < index 1
    CHECK(v1 < v4);
    CHECK(v4 > v1);

    // operator<=>
    auto cmp = v1 <=> v2;
    CHECK(cmp == 0);
    auto cmp2 = v1 <=> v3;
    CHECK(cmp2 < 0);
}

// ============================================================================
// test_variant_monostate
// ============================================================================
void test_variant_monostate() {
    std::monostate m1, m2;
    CHECK(m1 == m2);
    CHECK(!(m1 != m2));

    std::variant<std::monostate, int> v;
    CHECK(v.index() == 0);
    CHECK(std::holds_alternative<std::monostate>(v));

    // Compare monostate
    auto cmp = m1 <=> m2;
    CHECK(cmp == 0);
}

// ============================================================================
// test_variant_npos
// ============================================================================
void test_variant_npos() {
    static_assert(std::variant_npos == static_cast<std::size_t>(-1));
}

// ============================================================================
// test_variant_complex_types
// ============================================================================

struct NonTrivial {
    int x;
    explicit NonTrivial(int v) : x(v) {}
    NonTrivial(const NonTrivial& o) : x(o.x) {}
    NonTrivial(NonTrivial&& o) noexcept : x(o.x) { o.x = -1; }
    NonTrivial& operator=(const NonTrivial& o) { x = o.x; return *this; }
    NonTrivial& operator=(NonTrivial&& o) noexcept { x = o.x; o.x = -1; return *this; }
    ~NonTrivial() {}
    bool operator==(const NonTrivial& o) const { return x == o.x; }
    bool operator<(const NonTrivial& o) const { return x < o.x; }
};

void test_variant_complex_types() {
    std::variant<NonTrivial, int> v(std::in_place_type<NonTrivial>, 42);
    CHECK(v.index() == 0);
    CHECK(std::get<NonTrivial>(v).x == 42);

    // Copy
    std::variant<NonTrivial, int> v2 = v;
    CHECK(std::get<NonTrivial>(v2).x == 42);

    // Move
    std::variant<NonTrivial, int> v3 = std::move(v);
    CHECK(std::get<NonTrivial>(v3).x == 42);
    // v.get<0>().x == -1 after move

    // Assignment changes index
    v3 = 99;
    CHECK(v3.index() == 1);
    CHECK(std::get<int>(v3) == 99);
}

// ============================================================================
// test_variant_free_swap
// ============================================================================
void test_variant_free_swap() {
    std::variant<int, double> v1(10);
    std::variant<int, double> v2(2.5);
    std::swap(v1, v2);
    CHECK(v1.index() == 1);
    CHECK(std::get<double>(v1) == 2.5);
    CHECK(v2.index() == 0);
    CHECK(std::get<int>(v2) == 10);
}

// ============================================================================
// test_variant_hash
// ============================================================================
void test_variant_hash() {
    // Equal variants (same index, same value) must hash equal.
    std::variant<int, double> a(42);
    std::variant<int, double> b(42);
    CHECK(std::hash<std::variant<int, double>>{}(a) ==
          std::hash<std::variant<int, double>>{}(b));

    // Equal variants with double alternative must hash equal.
    std::variant<int, double> c(3.14);
    std::variant<int, double> d(3.14);
    CHECK(std::hash<std::variant<int, double>>{}(c) ==
          std::hash<std::variant<int, double>>{}(d));

    // Different index -> different hash (int 1 vs double 1.0 — very likely different).
    std::variant<int, double> ia(1);
    std::variant<int, double> da(1.0);
    // Index differs (0 vs 1), so hashes should differ.
    CHECK(std::hash<std::variant<int, double>>{}(ia) !=
          std::hash<std::variant<int, double>>{}(da));

    // hash<monostate>: all monostates hash equal.
    CHECK(std::hash<std::monostate>{}(std::monostate{}) ==
          std::hash<std::monostate>{}(std::monostate{}));

    // hash<variant<monostate>>: equal variants hash equal.
    std::variant<std::monostate> m1;
    std::variant<std::monostate> m2;
    CHECK(std::hash<std::variant<std::monostate>>{}(m1) ==
          std::hash<std::variant<std::monostate>>{}(m2));

    // Static check: hash<variant<int,double>> is a valid specialisation.
    static_assert(std::is_default_constructible_v<
        std::hash<std::variant<int, double>>>);
}
