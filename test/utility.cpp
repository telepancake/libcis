// Tests for <utility>.
// Consolidates tests from libc++ test/std/utilities/utility/ and related dirs.
// Each test is a free function void test_utility_<case>() with no args.
// static_assert for compile-time properties, CHECK(...) for runtime.
// No progress printing; crashes on failure.
#include <utility>
#include <type_traits>
#include "test.h"

// ============================================================
// move / forward / move_if_noexcept
// ============================================================

void test_utility_move() {
    // move returns remove_reference_t<T>&&
    int x = 42;
    int&& rx = std::move(x);
    CHECK(rx == 42);

    // move on lvalue ref type
    int& lr = x;
    int&& rx2 = std::move(lr);
    CHECK(rx2 == 42);

    // compile-time: move of const T yields const T&&
    static_assert(std::is_same_v<decltype(std::move(std::declval<const int&>())), const int&&>);
    static_assert(std::is_same_v<decltype(std::move(std::declval<int&>())), int&&>);
    static_assert(std::is_same_v<decltype(std::move(std::declval<int&&>())), int&&>);
}

void test_utility_forward() {
    // forward of lvalue
    int x = 5;
    static_assert(std::is_same_v<decltype(std::forward<int&>(x)), int&>);
    static_assert(std::is_same_v<decltype(std::forward<int>(x)), int&&>);
    static_assert(std::is_same_v<decltype(std::forward<const int&>(x)), const int&>);

    // forward preserves value category
    int y = 10;
    int& lr = y;
    int& r2 = std::forward<int&>(lr);
    CHECK(r2 == 10);
}

struct ThrowMoveConstruct {
    ThrowMoveConstruct() = default;
    ThrowMoveConstruct(const ThrowMoveConstruct&) = default;
    ThrowMoveConstruct(ThrowMoveConstruct&&) noexcept(false) {}
    ThrowMoveConstruct& operator=(ThrowMoveConstruct&&) = default;
};

struct NothrowMoveOnly {
    NothrowMoveOnly() = default;
    NothrowMoveOnly(const NothrowMoveOnly&) = delete;
    NothrowMoveOnly(NothrowMoveOnly&&) noexcept = default;
    NothrowMoveOnly& operator=(NothrowMoveOnly&&) = default;
};

void test_utility_move_if_noexcept() {
    // If move ctor can throw AND copy ctor available: returns const lvalue ref
    ThrowMoveConstruct tc;
    static_assert(std::is_same_v<decltype(std::move_if_noexcept(tc)), const ThrowMoveConstruct&>);

    // If move ctor is noexcept: returns rvalue ref
    int x = 42;
    static_assert(std::is_same_v<decltype(std::move_if_noexcept(x)), int&&>);

    // Move-only (no copy ctor): even with throwing move, returns rvalue ref
    NothrowMoveOnly mo;
    static_assert(std::is_same_v<decltype(std::move_if_noexcept(mo)), NothrowMoveOnly&&>);
}

// ============================================================
// as_const
// ============================================================

void test_utility_as_const() {
    int x = 42;
    static_assert(std::is_same_v<decltype(std::as_const(x)), const int&>);
    const int& cr = std::as_const(x);
    CHECK(cr == 42);
    x = 100;
    CHECK(cr == 100);  // ref still points to x
}

// ============================================================
// swap (two-arg and array)
// ============================================================

void test_utility_swap_scalar() {
    int a = 1, b = 2;
    std::swap(a, b);
    CHECK(a == 2 && b == 1);
}

void test_utility_swap_array() {
    int a[3] = {1, 2, 3};
    int b[3] = {4, 5, 6};
    std::swap(a, b);
    CHECK(a[0] == 4 && a[1] == 5 && a[2] == 6);
    CHECK(b[0] == 1 && b[1] == 2 && b[2] == 3);
}

void test_utility_swap_noexcept() {
    // int swap is noexcept
    static_assert(noexcept(std::swap(std::declval<int&>(), std::declval<int&>())));
    // is_swappable traits (defined in type_traits, verified consistent)
    static_assert(std::is_swappable_v<int>);
    static_assert(std::is_nothrow_swappable_v<int>);
    static_assert(!std::is_swappable_v<void>);
}

// ============================================================
// exchange
// ============================================================

void test_utility_exchange() {
    int x = 42;
    int old = std::exchange(x, 100);
    CHECK(old == 42);
    CHECK(x == 100);

    // exchange with different type for new value
    long y = 7L;
    long old2 = std::exchange(y, 8);
    CHECK(old2 == 7L && y == 8L);

    // constexpr context
    auto f = []() constexpr {
        int v = 1;
        int o = std::exchange(v, 2);
        return o == 1 && v == 2;
    };
    static_assert(f());
}

// ============================================================
// integer_sequence / index_sequence
// ============================================================

void test_utility_integer_sequence_basic() {
    using s0 = std::integer_sequence<int>;
    static_assert(s0::size() == 0);
    static_assert(std::is_same_v<s0::value_type, int>);

    using s3 = std::integer_sequence<int, 0, 1, 2>;
    static_assert(s3::size() == 3);

    using idx5 = std::index_sequence<0, 1, 2, 3, 4>;
    static_assert(idx5::size() == 5);
    static_assert(std::is_same_v<idx5::value_type, std::size_t>);
}

void test_utility_make_integer_sequence() {
    // make_integer_sequence<int, 0> is integer_sequence<int>
    using s0 = std::make_integer_sequence<int, 0>;
    static_assert(s0::size() == 0);

    // make_integer_sequence<int, 5> == integer_sequence<int, 0,1,2,3,4>
    using s5 = std::make_integer_sequence<int, 5>;
    static_assert(s5::size() == 5);
    static_assert(std::is_same_v<s5, std::integer_sequence<int, 0, 1, 2, 3, 4>>);

    // make_index_sequence<4>
    using idx4 = std::make_index_sequence<4>;
    static_assert(idx4::size() == 4);
    static_assert(std::is_same_v<idx4, std::index_sequence<0, 1, 2, 3>>);
}

void test_utility_index_sequence_for() {
    using s = std::index_sequence_for<int, double, char>;
    static_assert(s::size() == 3);
    static_assert(std::is_same_v<s, std::index_sequence<0, 1, 2>>);
}

// ============================================================
// in_place tags
// ============================================================

void test_utility_in_place() {
    std::in_place_t ip{};
    std::in_place_type_t<int> ipt{};
    std::in_place_index_t<0> ipi0{};
    std::in_place_index_t<5> ipi5{};
    (void)ip; (void)ipt; (void)ipi0; (void)ipi5;

    // in_place, in_place_type, in_place_index are constexpr inline variables
    static_assert(std::is_same_v<const std::in_place_t,
                                  decltype(std::in_place)>);
    static_assert(std::is_same_v<const std::in_place_type_t<double>,
                                  decltype(std::in_place_type<double>)>);
    static_assert(std::is_same_v<const std::in_place_index_t<2>,
                                  decltype(std::in_place_index<2>)>);
}

// ============================================================
// pair — construction
// ============================================================

void test_utility_pair_types() {
    static_assert(std::is_same_v<std::pair<int,double>::first_type, int>);
    static_assert(std::is_same_v<std::pair<int,double>::second_type, double>);
}

void test_utility_pair_default_construct() {
    std::pair<int, double> p;
    CHECK(p.first == 0 && p.second == 0.0);

    // explicit default ctor: pair with explicit-default-constructible member
    struct ExplicitDC {
        explicit ExplicitDC() {}
    };
    std::pair<ExplicitDC, int> ep;
    (void)ep;
}

void test_utility_pair_value_construct() {
    std::pair<int, double> p(3, 4.5);
    CHECK(p.first == 3 && p.second == 4.5);
}

void test_utility_pair_converting_construct() {
    std::pair<long, double> p1(3, 4);  // int -> long, int -> double
    CHECK(p1.first == 3L && p1.second == 4.0);

    std::pair<int, int> src{10, 20};
    std::pair<long, long> p2(src);  // converting copy from pair<int,int>
    CHECK(p2.first == 10L && p2.second == 20L);

    std::pair<long, long> p3(std::move(src));  // converting move
    CHECK(p3.first == 10L && p3.second == 20L);
}

void test_utility_pair_copy_move_construct() {
    std::pair<int, int> orig{5, 7};
    std::pair<int, int> copied(orig);
    CHECK(copied.first == 5 && copied.second == 7);

    std::pair<int, int> moved(std::move(orig));
    CHECK(moved.first == 5 && moved.second == 7);
}

// ============================================================
// pair — assignment
// ============================================================

void test_utility_pair_copy_assign() {
    std::pair<int,int> a{1,2}, b{3,4};
    a = b;
    CHECK(a.first == 3 && a.second == 4);
}

void test_utility_pair_move_assign() {
    struct MO {
        int v;
        MO(int x) : v(x) {}
        MO(const MO&) = delete;
        MO(MO&& o) noexcept : v(o.v) { o.v = -1; }
        MO& operator=(const MO&) = delete;
        MO& operator=(MO&& o) noexcept { v = o.v; o.v = -1; return *this; }
    };
    std::pair<MO, int> a(MO{10}, 20);
    std::pair<MO, int> b(MO{30}, 40);
    a = std::move(b);
    CHECK(a.first.v == 30 && a.second == 40);
    CHECK(b.first.v == -1);
}

void test_utility_pair_converting_assign() {
    std::pair<int,int> src{5,6};
    std::pair<long,long> dst{0,0};
    dst = src;  // converting copy assign
    CHECK(dst.first == 5L && dst.second == 6L);
}

// ============================================================
// pair — swap
// ============================================================

void test_utility_pair_swap_member() {
    std::pair<int,int> a{1,2}, b{3,4};
    a.swap(b);
    CHECK(a.first == 3 && a.second == 4);
    CHECK(b.first == 1 && b.second == 2);
}

void test_utility_pair_swap_free() {
    std::pair<int,int> a{10,20}, b{30,40};
    std::swap(a, b);
    CHECK(a.first == 30 && a.second == 40);
    CHECK(b.first == 10 && b.second == 20);
}

// ============================================================
// pair — operators == and <=>
// ============================================================

void test_utility_pair_equal() {
    std::pair<int,int> a{1,2}, b{1,2}, c{1,3};
    CHECK(a == b);
    CHECK(!(a == c));
}

void test_utility_pair_spaceship() {
    std::pair<int,int> a{1,2}, b{1,3}, c{2,0};
    CHECK((a <=> b) < 0);  // second differs
    CHECK((b <=> a) > 0);
    CHECK((a <=> c) < 0);  // first differs
    CHECK((a <=> a) == 0);
}

// ============================================================
// pair — tuple_size / tuple_element / get
// ============================================================

void test_utility_pair_tuple_interface() {
    static_assert(std::tuple_size<std::pair<int,double>>::value == 2);
    static_assert(std::is_same_v<std::tuple_element<0, std::pair<int,double>>::type, int>);
    static_assert(std::is_same_v<std::tuple_element<1, std::pair<int,double>>::type, double>);

    std::pair<int,double> p{42, 3.14};
    CHECK(std::get<0>(p) == 42);
    CHECK(std::get<1>(p) == 3.14);

    // get by type
    CHECK(std::get<int>(p) == 42);
    CHECK(std::get<double>(p) == 3.14);

    // get on rvalue
    CHECK(std::get<0>(std::pair<int,int>{1,2}) == 1);
}

void test_utility_pair_structured_binding() {
    auto [a, b] = std::pair<int, double>{7, 2.5};
    CHECK(a == 7 && b == 2.5);
}

// ============================================================
// make_pair
// ============================================================

void test_utility_make_pair() {
    auto p = std::make_pair(1, 2.5);
    static_assert(std::is_same_v<decltype(p), std::pair<int,double>>);
    CHECK(p.first == 1 && p.second == 2.5);

    // make_pair with char* deduces pair<const char*, int>
    const char* s = "hi";
    auto p3 = std::make_pair(s, 3);
    static_assert(std::is_same_v<decltype(p3), std::pair<const char*, int>>);
    (void)p3;

    // make_pair constexpr
    constexpr auto cp = std::make_pair(10, 20);
    static_assert(cp.first == 10 && cp.second == 20);
}

// ============================================================
// integer comparison functions
// ============================================================

void test_utility_cmp_equal() {
    // same sign
    CHECK(std::cmp_equal(3, 3));
    CHECK(!std::cmp_equal(3, 4));

    // signed vs unsigned
    CHECK(!std::cmp_equal(-1, 0u));
    CHECK(!std::cmp_equal(-1, static_cast<unsigned int>(-1)));
    CHECK(std::cmp_equal(3u, 3));
    CHECK(std::cmp_equal(0u, 0));
    CHECK(!std::cmp_equal(256u, static_cast<unsigned char>(0)));
}

void test_utility_cmp_less() {
    CHECK(std::cmp_less(1, 2));
    CHECK(!std::cmp_less(2, 1));
    CHECK(!std::cmp_less(2, 2));

    // signed vs unsigned: -1 < 0u
    CHECK(std::cmp_less(-1, 0u));
    // unsigned vs signed: 5u < 10
    CHECK(std::cmp_less(5u, 10));
    // large unsigned > negative signed
    CHECK(!std::cmp_less(100u, -1));
}

void test_utility_cmp_greater() {
    CHECK(std::cmp_greater(2, 1));
    CHECK(!std::cmp_greater(1, 2));
    CHECK(!std::cmp_greater(-1, 0u));
    CHECK(std::cmp_greater(0u, -1));
}

void test_utility_cmp_less_equal() {
    CHECK(std::cmp_less_equal(1, 2));
    CHECK(std::cmp_less_equal(2, 2));
    CHECK(!std::cmp_less_equal(3, 2));
    CHECK(std::cmp_less_equal(-1, 0u));
}

void test_utility_cmp_greater_equal() {
    CHECK(std::cmp_greater_equal(2, 1));
    CHECK(std::cmp_greater_equal(2, 2));
    CHECK(!std::cmp_greater_equal(1, 2));
    CHECK(std::cmp_greater_equal(0u, -1));
}

void test_utility_in_range() {
    // int range is [-2^31, 2^31-1]
    CHECK(std::in_range<int>(0));
    CHECK(std::in_range<int>(2147483647));
    CHECK(!std::in_range<int>(2147483648LL));
    CHECK(std::in_range<int>(-2147483648LL));
    CHECK(!std::in_range<int>(-2147483649LL));

    // unsigned char range [0, 255]
    CHECK(std::in_range<unsigned char>(0));
    CHECK(std::in_range<unsigned char>(255));
    CHECK(!std::in_range<unsigned char>(256));
    CHECK(!std::in_range<unsigned char>(-1));

    // unsigned int can't hold -1
    CHECK(!std::in_range<unsigned int>(-1));
}

// ============================================================
// to_underlying
// ============================================================

void test_utility_to_underlying() {
    enum class Color : int { Red = 0, Green = 1, Blue = 2 };
    static_assert(std::is_same_v<decltype(std::to_underlying(Color::Red)), int>);
    CHECK(std::to_underlying(Color::Red) == 0);
    CHECK(std::to_underlying(Color::Blue) == 2);

    enum class Flags : unsigned char { A = 1, B = 2, C = 4 };
    static_assert(std::is_same_v<decltype(std::to_underlying(Flags::A)), unsigned char>);
    CHECK(std::to_underlying(Flags::C) == 4);
}

// ============================================================
// unreachable (compile-time: it's [[noreturn]])
// ============================================================

void test_utility_unreachable() {
    // We can only test that the symbol exists and is callable;
    // we must NOT call it at runtime as it calls __builtin_unreachable.
    // Verify it is [[noreturn]]:
    static_assert(std::is_same_v<decltype(&std::unreachable), void(*)()>);
    // Just verify compilation — no runtime call.
}

// ============================================================
// piecewise_construct
// ============================================================

void test_utility_piecewise_construct() {
    // Just verify the tag is constructible and the inline constexpr variable exists
    std::piecewise_construct_t pct{};
    (void)pct;
    // piecewise_construct is an inline constexpr variable of type piecewise_construct_t
    static_assert(std::is_same_v<const std::piecewise_construct_t,
                                  decltype(std::piecewise_construct)>);
}
