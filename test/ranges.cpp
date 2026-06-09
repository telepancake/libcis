// ranges.cpp — tests for <ranges>
// Each test_ function is auto-discovered by tools/gen_main.sh.
// No main() here. Use CHECK() for runtime, static_assert for compile-time.
#include "test.h"
#include <ranges>
#include <iterator>
#include <type_traits>
#include <concepts>

// ---- concepts: range, sized_range, borrowed_range, view, etc. ----

void test_range_concepts_array() {
    int arr[5] = {1, 2, 3, 4, 5};
    static_assert(std::ranges::range<int(&)[5]>);
    static_assert(std::ranges::sized_range<int(&)[5]>);
    static_assert(std::ranges::borrowed_range<int(&)[5]>);
    static_assert(std::ranges::forward_range<int(&)[5]>);
    static_assert(std::ranges::bidirectional_range<int(&)[5]>);
    static_assert(std::ranges::random_access_range<int(&)[5]>);
    static_assert(std::ranges::contiguous_range<int(&)[5]>);
    static_assert(std::ranges::common_range<int(&)[5]>);
    (void)arr;
}

// ---- CPOs: begin / end / size / empty / data / rbegin / rend ----

void test_cpos_on_array() {
    int arr[5] = {10, 20, 30, 40, 50};

    auto* b = std::ranges::begin(arr);
    auto* e = std::ranges::end(arr);
    CHECK(*b == 10);
    CHECK(*(e - 1) == 50);
    CHECK(std::ranges::size(arr) == 5u);
    CHECK(!std::ranges::empty(arr));
    CHECK(std::ranges::data(arr) == arr);

    // empty check via iota (forward range, no .empty(), use begin==end)
    auto empty_iota = std::views::iota(5, 5);
    CHECK(std::ranges::empty(empty_iota));
}

void test_cpos_cbegin_cend() {
    int arr[3] = {1, 2, 3};
    auto* cb = std::ranges::cbegin(arr);
    auto* ce = std::ranges::cend(arr);
    static_assert(std::is_same_v<decltype(cb), const int*>);
    CHECK(*cb == 1);
    CHECK(*(ce - 1) == 3);
}

void test_cpos_rbegin_rend() {
    int arr[4] = {1, 2, 3, 4};
    auto rb = std::ranges::rbegin(arr);
    auto re = std::ranges::rend(arr);
    CHECK(*rb == 4);
    CHECK(*(re - 1) == 1); // reverse_iterator arithmetic
}

void test_cpos_ssize() {
    int arr[7] = {};
    auto s = std::ranges::ssize(arr);
    static_assert(std::is_signed_v<decltype(s)>);
    CHECK(s == 7);
}

// ---- struct with member begin/end ----

namespace {
struct simple_range {
    int data_[4] = {10, 20, 30, 40};
    int* begin() { return data_; }
    int* end()   { return data_ + 4; }
    std::size_t size() const { return 4; }
};
} // anonymous namespace

void test_cpos_member() {
    simple_range sr;
    CHECK(*std::ranges::begin(sr) == 10);
    CHECK(std::ranges::size(sr) == 4u);
    CHECK(!std::ranges::empty(sr));
}

// ---- ranges::advance, distance, next, prev ----

void test_advance_distance_next_prev() {
    int arr[5] = {0, 1, 2, 3, 4};
    auto* it = arr;

    std::ranges::advance(it, 3);
    CHECK(*it == 3);

    std::ranges::advance(it, -2);
    CHECK(*it == 1);

    auto d = std::ranges::distance(arr, arr + 5);
    CHECK(d == 5);

    auto d2 = std::ranges::distance(arr);
    CHECK(d2 == 5);

    auto nxt = std::ranges::next(arr, 2);
    CHECK(*nxt == 2);

    auto prv = std::ranges::prev(arr + 4, 2);
    CHECK(*prv == 2);
}

// ---- iota_view ----

void test_iota_view_basic() {
    // finite iota [0, 5)
    auto v = std::views::iota(0, 5);
    static_assert(std::ranges::range<decltype(v)>);
    static_assert(std::ranges::sized_range<decltype(v)>);
    CHECK(std::ranges::size(v) == 5u);

    int sum = 0;
    for (auto x : v) sum += x;
    CHECK(sum == 10); // 0+1+2+3+4

    CHECK(*std::ranges::begin(v) == 0);
}

void test_iota_view_infinite() {
    auto v = std::views::iota(1);
    auto it = std::ranges::begin(v);
    CHECK(*it == 1);
    ++it;
    CHECK(*it == 2);
    ++it;
    ++it;
    CHECK(*it == 4);
}

// ---- empty_view / single_view ----

void test_empty_view() {
    auto ev = std::views::empty<int>;
    static_assert(std::ranges::range<decltype(ev)>);
    CHECK(std::ranges::empty(ev));
    CHECK(std::ranges::size(ev) == 0u);
    CHECK(std::ranges::begin(ev) == nullptr);
}

void test_single_view() {
    auto sv = std::views::single(42);
    static_assert(std::ranges::range<decltype(sv)>);
    CHECK(!std::ranges::empty(sv));
    CHECK(std::ranges::size(sv) == 1u);
    CHECK(*std::ranges::begin(sv) == 42);
}

// ---- ref_view ----

void test_ref_view() {
    int arr[3] = {7, 8, 9};
    auto rv = std::ranges::ref_view(arr);
    static_assert(std::ranges::borrowed_range<decltype(rv)>);
    CHECK(*std::ranges::begin(rv) == 7);
    CHECK(std::ranges::size(rv) == 3u);
}

// ---- views::all ----

void test_views_all() {
    int arr[3] = {1, 2, 3};
    // lvalue — should produce ref_view
    auto a = std::views::all(arr);
    static_assert(std::ranges::view<decltype(a)>);
    CHECK(*std::ranges::begin(a) == 1);
    CHECK(std::ranges::size(a) == 3u);
}

// ---- transform_view ----

void test_transform_view_array() {
    int arr[4] = {1, 2, 3, 4};
    auto tv = arr | std::views::transform([](int x) { return x * x; });
    static_assert(std::ranges::range<decltype(tv)>);

    auto it = std::ranges::begin(tv);
    CHECK(*it == 1);
    ++it; CHECK(*it == 4);
    ++it; CHECK(*it == 9);
    ++it; CHECK(*it == 16);
}

void test_transform_view_size() {
    int arr[5] = {0, 1, 2, 3, 4};
    auto tv = arr | std::views::transform([](int x) { return x + 1; });
    CHECK(std::ranges::size(tv) == 5u);
}

// ---- filter_view ----

void test_filter_view_basic() {
    int arr[6] = {1, 2, 3, 4, 5, 6};
    auto fv = arr | std::views::filter([](int x) { return x % 2 == 0; });
    static_assert(std::ranges::range<decltype(fv)>);

    int vals[3];
    int i = 0;
    for (auto x : fv) vals[i++] = x;
    CHECK(i == 3);
    CHECK(vals[0] == 2);
    CHECK(vals[1] == 4);
    CHECK(vals[2] == 6);
}

// ---- take_view ----

void test_take_view_basic() {
    int arr[6] = {10, 20, 30, 40, 50, 60};
    auto tv = arr | std::views::take(3);
    static_assert(std::ranges::range<decltype(tv)>);

    int count = 0;
    int first = -1;
    for (auto x : tv) {
        if (count == 0) first = x;
        ++count;
    }
    CHECK(count == 3);
    CHECK(first == 10);
}

// ---- drop_view ----

void test_drop_view_basic() {
    int arr[6] = {10, 20, 30, 40, 50, 60};
    auto dv = arr | std::views::drop(2);
    static_assert(std::ranges::range<decltype(dv)>);

    auto it = std::ranges::begin(dv);
    CHECK(*it == 30);

    int count = 0;
    for (auto x : dv) { (void)x; ++count; }
    CHECK(count == 4);
}

void test_drop_view_size() {
    int arr[6] = {};
    auto dv = arr | std::views::drop(2);
    CHECK(std::ranges::size(dv) == 4u);

    auto dv2 = arr | std::views::drop(10);
    CHECK(std::ranges::size(dv2) == 0u);
}

// ---- take_while_view ----

void test_take_while_view_basic() {
    int arr[6] = {1, 2, 3, 10, 11, 12};
    auto twv = arr | std::views::take_while([](int x) { return x < 5; });
    static_assert(std::ranges::range<decltype(twv)>);

    int count = 0;
    for (auto x : twv) { (void)x; ++count; }
    CHECK(count == 3);
}

// ---- drop_while_view ----

void test_drop_while_view_basic() {
    int arr[6] = {1, 2, 3, 10, 11, 12};
    auto dwv = arr | std::views::drop_while([](int x) { return x < 5; });
    static_assert(std::ranges::range<decltype(dwv)>);

    auto it = std::ranges::begin(dwv);
    CHECK(*it == 10);

    int count = 0;
    for (auto x : dwv) { (void)x; ++count; }
    CHECK(count == 3);
}

// ---- reverse_view ----

void test_reverse_view_basic() {
    int arr[4] = {1, 2, 3, 4};
    auto rv = arr | std::views::reverse;
    static_assert(std::ranges::range<decltype(rv)>);

    auto it = std::ranges::begin(rv);
    CHECK(*it == 4);
    ++it; CHECK(*it == 3);
    ++it; CHECK(*it == 2);
    ++it; CHECK(*it == 1);
}

void test_reverse_view_size() {
    int arr[5] = {};
    auto rv = arr | std::views::reverse;
    CHECK(std::ranges::size(rv) == 5u);
}

// ---- subrange ----

void test_subrange_basic() {
    int arr[5] = {1, 2, 3, 4, 5};
    auto sr = std::ranges::subrange(arr, arr + 3);
    static_assert(std::ranges::range<decltype(sr)>);
    static_assert(std::ranges::borrowed_range<decltype(sr)>);

    CHECK(*sr.begin() == 1);
    CHECK(sr.end() == arr + 3);
    CHECK(!sr.empty());

    int count = 0;
    for (auto x : sr) { (void)x; ++count; }
    CHECK(count == 3);
}

void test_subrange_sized() {
    int arr[5] = {10, 20, 30, 40, 50};
    std::ranges::subrange<int*, int*, std::ranges::subrange_kind::sized> sr(arr, arr + 4, 4);
    CHECK(sr.size() == 4u);
    CHECK(*sr.begin() == 10);
}

void test_subrange_advance() {
    int arr[5] = {1, 2, 3, 4, 5};
    auto sr = std::ranges::subrange(arr, arr + 5);
    sr.advance(2);
    CHECK(*sr.begin() == 3);
}

// ---- pipe operator composition ----

void test_pipe_composition() {
    int arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    // chain: filter evens, transform *2, take 3
    auto pipeline = std::views::filter([](int x) { return x % 2 == 0; })
                  | std::views::transform([](int x) { return x * 2; })
                  | std::views::take(3);
    auto result = arr | pipeline;

    int vals[3];
    int i = 0;
    for (auto x : result) vals[i++] = x;
    CHECK(i == 3);
    CHECK(vals[0] == 4);   // 2*2
    CHECK(vals[1] == 8);   // 4*2
    CHECK(vals[2] == 12);  // 6*2
}

// ---- views::counted ----

void test_views_counted() {
    int arr[5] = {10, 20, 30, 40, 50};
    // views::counted requires a non-reference iterator
    auto cv = std::views::counted(arr + 1, 3);
    static_assert(std::ranges::range<decltype(cv)>);

    auto cit = std::ranges::begin(cv);
    CHECK(*cit == 20);
    ++cit; CHECK(*cit == 30);
    ++cit; CHECK(*cit == 40);
}

// ---- dangling ----

void test_dangling() {
    // iota_view IS a borrowed range (enable_borrowed_range<iota_view<...>> = true)
    static_assert(std::ranges::borrowed_range<std::ranges::iota_view<int>>);
    // subrange is also borrowed
    static_assert(std::ranges::borrowed_range<std::ranges::subrange<int*>>);
    // borrowed_iterator_t of a borrowed range -> real iterator
    static_assert(std::is_same_v<std::ranges::borrowed_iterator_t<std::ranges::subrange<int*>>,
                                 int*>);
    // dangling is constructible
    std::ranges::dangling d;
    (void)d;
}

// ---- enable_view / view concept ----

void test_enable_view() {
    static_assert(std::ranges::view<std::ranges::empty_view<int>>);
    static_assert(std::ranges::view<std::ranges::single_view<int>>);
    static_assert(std::ranges::view<std::ranges::iota_view<int, int>>);
    static_assert(std::ranges::view<std::ranges::subrange<int*>>);
    // raw array is NOT a view
    static_assert(!std::ranges::view<int[5]>);
}

// ---- transform + iota pipeline ----

void test_iota_transform_pipeline() {
    auto r = std::views::iota(0, 5) | std::views::transform([](int x) { return x * x; });
    int vals[5];
    int i = 0;
    for (auto x : r) vals[i++] = x;
    CHECK(i == 5);
    CHECK(vals[0] == 0);
    CHECK(vals[1] == 1);
    CHECK(vals[2] == 4);
    CHECK(vals[3] == 9);
    CHECK(vals[4] == 16);
}
