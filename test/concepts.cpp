// test/concepts.cpp — tests for <concepts>
// One test function per concept group; all prefixed test_concepts_
// so names never collide with other test files.
// Compile-time checks: static_assert. Runtime checks: CHECK(expr).
#include <concepts>
#include "test.h"

// A minimal move-only type (replaces std::unique_ptr which requires <memory>)
namespace {
    struct MoveOnly {
        MoveOnly() = default;
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;
    };
}

// ─── same_as ───────────────────────────────────────────────────────────────
void test_concepts_same_as() {
    static_assert( std::same_as<int, int>);
    static_assert(!std::same_as<int, long>);
    static_assert( std::same_as<const int, const int>);
    static_assert(!std::same_as<int, const int>);
    static_assert( std::same_as<void, void>);
}

// ─── derived_from ──────────────────────────────────────────────────────────
namespace {
    struct Base {};
    struct Derived : Base {};
    struct Unrelated {};
}
void test_concepts_derived_from() {
    static_assert( std::derived_from<Derived, Base>);
    static_assert( std::derived_from<Derived, Derived>);
    static_assert(!std::derived_from<Base, Derived>);
    static_assert(!std::derived_from<Unrelated, Base>);
    static_assert(!std::derived_from<int, int>);
}

// ─── convertible_to ────────────────────────────────────────────────────────
void test_concepts_convertible_to() {
    static_assert( std::convertible_to<int, long>);
    static_assert( std::convertible_to<float, double>);
    static_assert(!std::convertible_to<int*, float*>);
    static_assert( std::convertible_to<int, int>);
    static_assert(!std::convertible_to<void, int>);
    static_assert( std::convertible_to<Derived*, Base*>);
}

// ─── arithmetic concepts ───────────────────────────────────────────────────
void test_concepts_arithmetic() {
    static_assert( std::integral<int>);
    static_assert( std::integral<unsigned int>);
    static_assert( std::integral<char>);
    static_assert( std::integral<bool>);
    static_assert(!std::integral<float>);
    static_assert(!std::integral<double>);

    static_assert( std::signed_integral<int>);
    static_assert( std::signed_integral<long>);
    static_assert(!std::signed_integral<unsigned int>);
    static_assert(!std::signed_integral<bool>);   // bool is not signed

    static_assert( std::unsigned_integral<unsigned int>);
    static_assert( std::unsigned_integral<unsigned long>);
    static_assert(!std::unsigned_integral<int>);

    static_assert( std::floating_point<float>);
    static_assert( std::floating_point<double>);
    static_assert( std::floating_point<long double>);
    static_assert(!std::floating_point<int>);
}

// ─── destructible ──────────────────────────────────────────────────────────
namespace {
    struct Indestructible { ~Indestructible() = delete; };
}
void test_concepts_destructible() {
    static_assert( std::destructible<int>);
    static_assert( std::destructible<Base>);
    static_assert(!std::destructible<Indestructible>);
    static_assert(!std::destructible<void>);
}

// ─── constructible_from ────────────────────────────────────────────────────
void test_concepts_constructible_from() {
    static_assert( std::constructible_from<int>);
    static_assert( std::constructible_from<int, int>);
    static_assert( std::constructible_from<int, long>);
    static_assert(!std::constructible_from<int&, int>);   // can't construct ref from non-ref
    static_assert( std::constructible_from<std::nullptr_t>);
}

// ─── default_initializable ─────────────────────────────────────────────────
namespace {
    struct NoDefaultCtor { NoDefaultCtor() = delete; };
}
void test_concepts_default_initializable() {
    static_assert( std::default_initializable<int>);
    static_assert( std::default_initializable<double>);
    static_assert( std::default_initializable<Base>);
    static_assert(!std::default_initializable<NoDefaultCtor>);
    static_assert(!std::default_initializable<int&>);
}

// ─── move_constructible ────────────────────────────────────────────────────
namespace {
    struct NoMove {
        NoMove()             = default;
        NoMove(const NoMove&)= default;
        NoMove(NoMove&&)     = delete;
    };
}
void test_concepts_move_constructible() {
    static_assert( std::move_constructible<int>);
    static_assert( std::move_constructible<std::nullptr_t>);
    static_assert(!std::move_constructible<NoMove>);
}

// ─── copy_constructible ────────────────────────────────────────────────────
namespace {
    struct NoCopy {
        NoCopy()             = default;
        NoCopy(NoCopy&&)     = default;
        NoCopy(const NoCopy&)= delete;
    };
}
void test_concepts_copy_constructible() {
    static_assert( std::copy_constructible<int>);
    static_assert(!std::copy_constructible<NoCopy>);
    static_assert(!std::copy_constructible<MoveOnly>);
}

// ─── assignable_from ───────────────────────────────────────────────────────
void test_concepts_assignable_from() {
    static_assert( std::assignable_from<int&, int>);
    static_assert( std::assignable_from<int&, long>);
    static_assert(!std::assignable_from<int, int>);   // lhs must be lvalue ref
    static_assert( std::assignable_from<Base&, Base>);
}

// ─── swappable ─────────────────────────────────────────────────────────────
void test_concepts_swappable() {
    static_assert( std::swappable<int>);
    static_assert( std::swappable<double>);
    static_assert( std::swappable<Base>);
    // Arrays are swappable if element type is
    static_assert( std::swappable<int[3]>);
}

// ─── common_reference_with ─────────────────────────────────────────────────
void test_concepts_common_reference_with() {
    static_assert( std::common_reference_with<int&, int&>);
    static_assert( std::common_reference_with<int, int>);
    static_assert( std::common_reference_with<int, long>);
    // int& and const int& — common reference is const int&
    static_assert( std::common_reference_with<int&, const int&>);
}

// ─── common_with ───────────────────────────────────────────────────────────
void test_concepts_common_with() {
    static_assert( std::common_with<int, int>);
    static_assert( std::common_with<int, long>);
    static_assert( std::common_with<float, double>);
}

// ─── equality_comparable ───────────────────────────────────────────────────
void test_concepts_equality_comparable() {
    static_assert( std::equality_comparable<int>);
    static_assert( std::equality_comparable<float>);
    static_assert( std::equality_comparable<int*>);
    // Pointers to different types are not equality-comparable with each other
    // int and long are equality_comparable_with (via common type)
    static_assert( std::equality_comparable_with<int, long>);
}

// ─── totally_ordered ───────────────────────────────────────────────────────
void test_concepts_totally_ordered() {
    static_assert( std::totally_ordered<int>);
    static_assert( std::totally_ordered<float>);
    static_assert( std::totally_ordered<double>);
    static_assert( std::totally_ordered_with<int, long>);
    static_assert( std::totally_ordered_with<int, double>);
}

// ─── movable ───────────────────────────────────────────────────────────────
void test_concepts_movable() {
    static_assert( std::movable<int>);
    static_assert( std::movable<double>);
    static_assert(!std::movable<const int>);    // can't assign to const
    static_assert(!std::movable<int&>);          // not an object type
}

// ─── copyable ──────────────────────────────────────────────────────────────
void test_concepts_copyable() {
    static_assert( std::copyable<int>);
    static_assert( std::copyable<double>);
    static_assert(!std::copyable<MoveOnly>);  // move-only
}

// ─── semiregular ───────────────────────────────────────────────────────────
void test_concepts_semiregular() {
    static_assert( std::semiregular<int>);
    static_assert( std::semiregular<double>);
    static_assert(!std::semiregular<MoveOnly>);
}

// ─── regular ───────────────────────────────────────────────────────────────
void test_concepts_regular() {
    static_assert( std::regular<int>);
    static_assert( std::regular<double>);
    static_assert(!std::regular<MoveOnly>);
}

// ─── invocable / regular_invocable ─────────────────────────────────────────
namespace {
    int add(int a, int b) { return a + b; }
    struct Callable { int operator()(int x) const { return x * 2; } };
}
void test_concepts_invocable() {
    static_assert( std::invocable<decltype(add), int, int>);
    static_assert(!std::invocable<decltype(add), int>);
    static_assert( std::invocable<Callable, int>);
    // nullary function pointer
    using NullaryFn = int(*)();
    static_assert( std::invocable<NullaryFn>);
    // regular_invocable is the same constraint
    static_assert( std::regular_invocable<decltype(add), int, int>);
}

// ─── predicate ─────────────────────────────────────────────────────────────
namespace {
    bool is_positive(int x) { return x > 0; }
    int  not_a_pred(int x) { return x; }   // returns int, which IS convertible to bool
}
void test_concepts_predicate() {
    static_assert( std::predicate<decltype(is_positive), int>);
    // int is boolean-testable (convertible to bool), so this holds too
    static_assert( std::predicate<decltype(not_a_pred), int>);
    // A lambda returning bool
    auto even = [](int n) { return n % 2 == 0; };
    static_assert( std::predicate<decltype(even), int>);
}

// ─── relation / equivalence_relation / strict_weak_order ───────────────────
namespace {
    bool int_less(int a, int b) { return a < b; }
    bool int_eq(int a, int b)   { return a == b; }
}
void test_concepts_relation() {
    static_assert( std::relation<decltype(int_less), int, int>);
    static_assert( std::equivalence_relation<decltype(int_eq), int, int>);
    static_assert( std::strict_weak_order<decltype(int_less), int, int>);
}

// ─── runtime sanity checks ─────────────────────────────────────────────────
void test_concepts_runtime() {
    // ranges::swap
    int a = 10, b = 20;
    std::ranges::swap(a, b);
    CHECK(a == 20);
    CHECK(b == 10);

    // exchange (from <utility>, available via <concepts> -> <utility>)
    int x = 1;
    int old = std::exchange(x, 42);
    CHECK(old == 1);
    CHECK(x == 42);

    // forward / move (from <utility>)
    int val = 100;
    int&& rv = std::move(val);
    CHECK(rv == 100);
}
