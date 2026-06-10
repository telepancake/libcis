// ranges.cpp — tests for <ranges>
// Each test_ function is auto-discovered by tools/gen_main.sh.
// No main() here. Use CHECK() for runtime, static_assert for compile-time.
#include "test.h"
#include <ranges>
#include <iterator>
#include <type_traits>
#include <concepts>
#include <string_view>

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

// ---- join_view ----

namespace {
// Simple flat array range that is a view
struct int_array_view : std::ranges::view_interface<int_array_view> {
    const int* data_;
    int size_;
    constexpr int_array_view(const int* d, int s) : data_(d), size_(s) {}
    constexpr const int* begin() const { return data_; }
    constexpr const int* end()   const { return data_ + size_; }
};

// Simple range of int_array_view
struct two_arrays {
    int_array_view sub[2];
    constexpr const int_array_view* begin() const { return sub; }
    constexpr const int_array_view* end()   const { return sub + 2; }
};
} // anonymous namespace

void test_ranges_join_view() {
    // join two int arrays to get flat sequence: 1,2,3,4,5,6
    static constexpr int a1[] = {1, 2, 3};
    static constexpr int a2[] = {4, 5, 6};
    two_arrays ta{{{a1, 3}, {a2, 3}}};
    auto jv = ta | std::views::join;
    static_assert(std::ranges::range<decltype(jv)>);

    int flat[6];
    int idx = 0;
    for (auto x : jv) flat[idx++] = x;

    CHECK(idx == 6);
    CHECK(flat[0] == 1);
    CHECK(flat[1] == 2);
    CHECK(flat[2] == 3);
    CHECK(flat[3] == 4);
    CHECK(flat[4] == 5);
    CHECK(flat[5] == 6);
}

void test_ranges_join_view_empty_inner() {
    // Some inner ranges empty: join yields 10, 20, 30
    static constexpr int e[] = {};
    static constexpr int a1[] = {10, 20};
    static constexpr int a2[] = {30};

    struct four_arrays {
        int_array_view sub[4];
        constexpr const int_array_view* begin() const { return sub; }
        constexpr const int_array_view* end()   const { return sub + 4; }
    };
    four_arrays ta{{{e, 0}, {a1, 2}, {e, 0}, {a2, 1}}};

    auto jv = ta | std::views::join;
    int flat[3];
    int idx = 0;
    for (auto x : jv) flat[idx++] = x;
    CHECK(idx == 3);
    CHECK(flat[0] == 10);
    CHECK(flat[1] == 20);
    CHECK(flat[2] == 30);
}

// ---- split_view ----

// Helper: collect subranges from a split into an array of strings
// We'll use std::string_view-like approach with char arrays.

void test_ranges_split_view() {
    // Split a string-like range on a delimiter char
    // Use a simple array of chars as the range, split on ','
    std::string_view sv = "hello,world,foo";
    auto parts = sv | std::views::split(',');
    static_assert(std::ranges::range<decltype(parts)>);

    // Collect the parts as string_views (each part is a subrange of iterators)
    int count = 0;
    for (auto part : parts) {
        (void)part;
        ++count;
    }
    CHECK(count == 3);

    // Check first part is "hello"
    auto it = std::ranges::begin(parts);
    auto first = *it;
    std::string_view expected = "hello";
    int i = 0;
    for (auto c : first) {
        CHECK(c == expected[i]);
        ++i;
    }
    CHECK(i == 5);
}

void test_ranges_lazy_split_view() {
    // lazy_split on a char range with single-char delimiter
    std::string_view sv = "a,b,c";
    auto parts = sv | std::views::lazy_split(',');
    static_assert(std::ranges::range<decltype(parts)>);

    int count = 0;
    for (auto part : parts) {
        (void)part;
        ++count;
    }
    CHECK(count == 3);
}

// ---- elements_view / keys_view / values_view ----

void test_ranges_elements_view() {
    // Array of pair<int,string_view> — use elements<0> (keys) and elements<1> (values)
    using P = std::pair<int, std::string_view>;
    P arr[3] = {{1, "one"}, {2, "two"}, {3, "three"}};

    // keys_view: yields 1, 2, 3
    auto ks = arr | std::views::keys;
    static_assert(std::ranges::range<decltype(ks)>);

    int k[3];
    int i = 0;
    for (auto x : ks) k[i++] = x;
    CHECK(i == 3);
    CHECK(k[0] == 1);
    CHECK(k[1] == 2);
    CHECK(k[2] == 3);

    // values_view: yields "one", "two", "three"
    auto vs = arr | std::views::values;
    static_assert(std::ranges::range<decltype(vs)>);
    auto vit = std::ranges::begin(vs);
    CHECK(*vit == "one");
    ++vit;
    CHECK(*vit == "two");
    ++vit;
    CHECK(*vit == "three");
}

void test_ranges_elements_view_static() {
    using P = std::pair<int, std::string_view>;
    // ref_view over array
    static_assert(std::ranges::range<std::ranges::keys_view<std::ranges::ref_view<P[3]>>>);
    static_assert(std::ranges::range<std::ranges::values_view<std::ranges::ref_view<P[3]>>>);
}

// ---- common_view ----

void test_ranges_common_view() {
    // take_while produces a non-common range (iterator != sentinel)
    // Wrap it in common_view so begin/end have same type
    int arr[6] = {1, 2, 3, 10, 11, 12};
    auto tw = arr | std::views::take_while([](int x) { return x < 5; });
    static_assert(!std::ranges::common_range<decltype(tw)>);

    // Apply common_view
    auto cv = tw | std::views::common;

    // Now it's a common range
    static_assert(std::ranges::range<decltype(cv)>);

    int vals[3];
    int count = 0;
    for (auto x : cv) vals[count++] = x;
    CHECK(count == 3);
    CHECK(vals[0] == 1);
    CHECK(vals[1] == 2);
    CHECK(vals[2] == 3);
}

void test_ranges_common_view_already_common() {
    // Applying views::common to an already-common range should return it unchanged (views::all)
    int arr[4] = {1, 2, 3, 4};
    auto cv = arr | std::views::common;
    // Should just be a ref_view (already common)
    static_assert(std::ranges::common_range<decltype(cv)>);
    int count = 0;
    for (auto x : cv) { (void)x; ++count; }
    CHECK(count == 4);
}
