// test/optional.cpp — tests for <optional>
// Conventions: void test_optional_<case>(), static_assert + CHECK, no main().
#include <optional>
#include <utility>
#include <type_traits>
#include "test.h"

// ---- helpers ----

struct Trivial {
    int x;
    constexpr Trivial(int v = 0) : x(v) {}
};

struct NonTrivial {
    int x;
    NonTrivial(int v = 0) : x(v) {}
    NonTrivial(const NonTrivial& o) : x(o.x) {}
    NonTrivial(NonTrivial&& o) : x(o.x) { o.x = -1; }
    NonTrivial& operator=(const NonTrivial& o) { x = o.x; return *this; }
    NonTrivial& operator=(NonTrivial&& o)      { x = o.x; o.x = -1; return *this; }
    ~NonTrivial() { x = 0; }
    bool operator==(const NonTrivial& o) const { return x == o.x; }
};

struct MoveOnly {
    int x;
    MoveOnly(int v = 0) : x(v) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&& o) : x(o.x) { o.x = -1; }
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly& operator=(MoveOnly&& o) { x = o.x; o.x = -1; return *this; }
};

// ---- tests ----

void test_optional_nullopt() {
    // nullopt_t is a type; nullopt is a valid constant
    static_assert(std::is_same_v<decltype(std::nullopt), const std::nullopt_t>);

    std::optional<int> a;
    CHECK(!a.has_value());
    CHECK(!a);

    std::optional<int> b(std::nullopt);
    CHECK(!b.has_value());
    CHECK(!b);
}

void test_optional_default_construct() {
    // default-constructed optional has no value
    std::optional<int> a;
    CHECK(!a);
    CHECK(!a.has_value());

    std::optional<Trivial> b;
    CHECK(!b);

    std::optional<NonTrivial> c;
    CHECK(!c);
}

void test_optional_value_construct() {
    // construct from value
    std::optional<int> a(42);
    CHECK(a.has_value());
    CHECK(*a == 42);

    std::optional<Trivial> b(Trivial{7});
    CHECK(b.has_value());
    CHECK(b->x == 7);

    std::optional<NonTrivial> c(NonTrivial{3});
    CHECK(c.has_value());
    CHECK(c->x == 3);
}

void test_optional_in_place() {
    std::optional<int> a(std::in_place, 99);
    CHECK(a.has_value());
    CHECK(*a == 99);

    std::optional<Trivial> b(std::in_place, 5);
    CHECK(b.has_value());
    CHECK(b->x == 5);

    std::optional<NonTrivial> c(std::in_place, 11);
    CHECK(c.has_value());
    CHECK(c->x == 11);
}

void test_optional_copy_construct() {
    std::optional<int> a(42);
    std::optional<int> b(a);
    CHECK(b.has_value());
    CHECK(*b == 42);

    std::optional<int> empty;
    std::optional<int> ec(empty);
    CHECK(!ec.has_value());

    std::optional<NonTrivial> c(NonTrivial{5});
    std::optional<NonTrivial> d(c);
    CHECK(d.has_value());
    CHECK(d->x == 5);
}

void test_optional_move_construct() {
    std::optional<int> a(42);
    std::optional<int> b(std::move(a));
    CHECK(b.has_value());
    CHECK(*b == 42);

    std::optional<MoveOnly> c(MoveOnly{7});
    std::optional<MoveOnly> d(std::move(c));
    CHECK(d.has_value());
    CHECK(d->x == 7);
}

void test_optional_converting_ctor() {
    // implicit converting from int to long
    std::optional<long> a(42);
    CHECK(a.has_value());
    CHECK(*a == 42L);

    // from optional<int> to optional<long>
    std::optional<int> oi(10);
    std::optional<long> ol(oi);
    CHECK(ol.has_value());
    CHECK(*ol == 10L);

    // from empty optional<int> to optional<long>
    std::optional<int> empty;
    std::optional<long> el(empty);
    CHECK(!el.has_value());
}

void test_optional_assign_nullopt() {
    std::optional<int> a(42);
    CHECK(a.has_value());
    a = std::nullopt;
    CHECK(!a.has_value());
}

void test_optional_copy_assign() {
    std::optional<int> a(42);
    std::optional<int> b;
    b = a;
    CHECK(b.has_value());
    CHECK(*b == 42);

    std::optional<int> empty;
    b = empty;
    CHECK(!b.has_value());

    std::optional<NonTrivial> c(NonTrivial{5});
    std::optional<NonTrivial> d;
    d = c;
    CHECK(d.has_value());
    CHECK(d->x == 5);
}

void test_optional_move_assign() {
    std::optional<int> a(42);
    std::optional<int> b;
    b = std::move(a);
    CHECK(b.has_value());
    CHECK(*b == 42);

    std::optional<MoveOnly> c(MoveOnly{7});
    std::optional<MoveOnly> d;
    d = std::move(c);
    CHECK(d.has_value());
    CHECK(d->x == 7);
}

void test_optional_value_assign() {
    std::optional<int> a;
    a = 42;
    CHECK(a.has_value());
    CHECK(*a == 42);

    // re-assign an engaged optional
    a = 99;
    CHECK(*a == 99);
}

void test_optional_emplace() {
    std::optional<NonTrivial> a;
    a.emplace(17);
    CHECK(a.has_value());
    CHECK(a->x == 17);

    // emplace on engaged optional resets first
    a.emplace(33);
    CHECK(a->x == 33);
}

void test_optional_reset() {
    std::optional<int> a(42);
    CHECK(a.has_value());
    a.reset();
    CHECK(!a.has_value());

    // reset of disengaged is a no-op
    a.reset();
    CHECK(!a.has_value());
}

void test_optional_swap() {
    std::optional<int> a(1), b(2);
    a.swap(b);
    CHECK(*a == 2);
    CHECK(*b == 1);

    std::optional<int> c(10), empty;
    c.swap(empty);
    CHECK(!c.has_value());
    CHECK(empty.has_value());
    CHECK(*empty == 10);

    std::swap(empty, c);
    CHECK(!empty.has_value());
    CHECK(*c == 10);
}

void test_optional_observers() {
    std::optional<int> a(42);
    CHECK(a.has_value());
    CHECK(static_cast<bool>(a));
    CHECK(*a == 42);

    const std::optional<int> ca(7);
    CHECK(*ca == 7);
    CHECK(ca.has_value());

    // operator->
    std::optional<Trivial> t(Trivial{3});
    CHECK(t->x == 3);

    const std::optional<Trivial> ct(Trivial{4});
    CHECK(ct->x == 4);
}

void test_optional_value_or() {
    std::optional<int> a(42);
    CHECK(a.value_or(0) == 42);

    std::optional<int> b;
    CHECK(b.value_or(99) == 99);

    // rvalue value_or
    CHECK(std::optional<int>(5).value_or(0) == 5);
    CHECK(std::optional<int>().value_or(7) == 7);
}

void test_optional_relops() {
    std::optional<int> a(1), b(2), c(1), empty1, empty2;

    // == / !=
    CHECK(a == c);
    CHECK(!(a == b));
    CHECK(a != b);
    CHECK(!(a != c));

    // empty comparisons
    CHECK(empty1 == empty2);
    CHECK(!(empty1 != empty2));
    CHECK(!(a == empty1));
    CHECK(a != empty1);

    // < / > / <= / >=
    CHECK(a < b);
    CHECK(!(b < a));
    CHECK(b > a);
    CHECK(a <= c);
    CHECK(a <= b);
    CHECK(b >= a);
    CHECK(c >= a);

    // nullopt comparisons
    CHECK(empty1 == std::nullopt);
    CHECK(std::nullopt == empty1);
    CHECK(!(a == std::nullopt));
    CHECK(a != std::nullopt);
}

void test_optional_compare_with_t() {
    std::optional<int> a(5);
    std::optional<int> empty;

    CHECK(a == 5);
    CHECK(5 == a);
    CHECK(!(a == 6));
    CHECK(a != 6);
    CHECK(6 != a);
    CHECK(!(empty == 5));
    CHECK(empty != 5);

    CHECK(a < 10);
    CHECK(!(a < 5));
    CHECK(a <= 5);
    CHECK(a <= 10);
    CHECK(a > 3);
    CHECK(a >= 5);
    CHECK(!(empty > 0));
    CHECK(empty < 0);
    CHECK(empty <= 0);
    CHECK(!(empty >= 0));
}

void test_optional_spaceship() {
    std::optional<int> a(1), b(2), c(1), empty;

    auto r1 = (a <=> b);
    CHECK(r1 < 0);

    auto r2 = (b <=> a);
    CHECK(r2 > 0);

    auto r3 = (a <=> c);
    CHECK(r3 == 0);

    auto r4 = (a <=> empty);
    CHECK(r4 > 0);

    auto r5 = (empty <=> a);
    CHECK(r5 < 0);

    auto r6 = (empty <=> empty);
    CHECK(r6 == 0);

    // compare with nullopt
    auto r7 = (a <=> std::nullopt);
    CHECK(r7 > 0);

    auto r8 = (empty <=> std::nullopt);
    CHECK(r8 == 0);

    // compare with T
    auto r9 = (a <=> 2);
    CHECK(r9 < 0);
    auto r10 = (a <=> 1);
    CHECK(r10 == 0);
    auto r11 = (empty <=> 1);
    CHECK(r11 < 0);
}

void test_optional_make_optional() {
    auto a = std::make_optional(42);
    static_assert(std::is_same_v<decltype(a), std::optional<int>>);
    CHECK(*a == 42);

    auto b = std::make_optional<NonTrivial>(7);
    static_assert(std::is_same_v<decltype(b), std::optional<NonTrivial>>);
    CHECK(b->x == 7);
}

void test_optional_and_then() {
    std::optional<int> a(5);
    auto r = a.and_then([](int x) -> std::optional<int> {
        return x * 2;
    });
    CHECK(r.has_value());
    CHECK(*r == 10);

    std::optional<int> empty;
    auto r2 = empty.and_then([](int x) -> std::optional<int> {
        return x * 2;
    });
    CHECK(!r2.has_value());
}

void test_optional_transform() {
    std::optional<int> a(5);
    auto r = a.transform([](int x) { return x * 3; });
    CHECK(r.has_value());
    CHECK(*r == 15);

    std::optional<int> empty;
    auto r2 = empty.transform([](int x) { return x * 3; });
    CHECK(!r2.has_value());

    // transform changes type
    std::optional<int> b(4);
    auto r3 = b.transform([](int x) { return static_cast<double>(x) * 0.5; });
    static_assert(std::is_same_v<decltype(r3), std::optional<double>>);
    CHECK(r3.has_value());
    CHECK(*r3 == 2.0);
}

void test_optional_or_else() {
    std::optional<int> a(5);
    auto r = a.or_else([]() -> std::optional<int> { return 99; });
    CHECK(r.has_value());
    CHECK(*r == 5);

    std::optional<int> empty;
    auto r2 = empty.or_else([]() -> std::optional<int> { return 99; });
    CHECK(r2.has_value());
    CHECK(*r2 == 99);
}

void test_optional_trivial_specialization() {
    // Trivial T should have trivially destructible optional
    static_assert(std::is_trivially_destructible_v<std::optional<Trivial>>);
    // Non-trivial T: NOT trivially destructible
    static_assert(!std::is_trivially_destructible_v<std::optional<NonTrivial>>);
}

void test_optional_static_asserts() {
    // has_value is consistent with operator bool
    std::optional<int> a(1);
    CHECK(a.has_value() == static_cast<bool>(a));

    std::optional<int> b;
    CHECK(b.has_value() == static_cast<bool>(b));

    // value_type
    static_assert(std::is_same_v<std::optional<int>::value_type, int>);
    static_assert(std::is_same_v<std::optional<NonTrivial>::value_type, NonTrivial>);
}

void test_optional_initializer_list() {
    // Test emplace with initializer_list
    struct VecLike {
        int sum = 0;
        VecLike(std::initializer_list<int> il) {
            for (int v : il) sum += v;
        }
    };

    std::optional<VecLike> a(std::in_place, {1, 2, 3});
    CHECK(a.has_value());
    CHECK(a->sum == 6);

    std::optional<VecLike> b;
    b.emplace({4, 5});
    CHECK(b.has_value());
    CHECK(b->sum == 9);
}

void test_optional_deduction_guide() {
    std::optional a(42);
    static_assert(std::is_same_v<decltype(a), std::optional<int>>);

    std::optional b(3.14);
    static_assert(std::is_same_v<decltype(b), std::optional<double>>);
}

void test_optional_hash() {
    // Two engaged optionals with equal values must hash equal.
    std::optional<int> a(42);
    std::optional<int> b(42);
    CHECK(std::hash<std::optional<int>>{}(a) == std::hash<std::optional<int>>{}(b));

    // Two disengaged optionals must hash equal (standard guarantees consistent hash
    // within a program execution; exact value is implementation-defined, not 0).
    std::optional<int> empty1;
    std::optional<int> empty2;
    CHECK(std::hash<std::optional<int>>{}(empty1) == std::hash<std::optional<int>>{}(empty2));

    // An engaged optional with a different value should (very likely) differ.
    std::optional<int> c(99);
    CHECK(std::hash<std::optional<int>>{}(a) != std::hash<std::optional<int>>{}(c));

    // Works for other hashable types (double).
    std::optional<double> d(3.14);
    std::optional<double> e(3.14);
    CHECK(std::hash<std::optional<double>>{}(d) == std::hash<std::optional<double>>{}(e));

    // hash<optional<T>> is disabled when hash<T> is disabled.
    // (Compile-time: optional<optional<int>> is hashable since optional<int> is hashable.)
    std::optional<std::optional<int>> nested(std::optional<int>(7));
    std::optional<std::optional<int>> nested2(std::optional<int>(7));
    CHECK(std::hash<std::optional<std::optional<int>>>{}(nested) ==
          std::hash<std::optional<std::optional<int>>>{}(nested2));
}
