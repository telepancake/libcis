// test/set.cpp — tests for <set> and <multiset>
// No #pragma once (test file)
#include <set>
#include "test.h"

// ---- test_set_basic ----
void test_set_basic() {
    std::set<int> s;
    CHECK(s.empty());
    CHECK(s.size() == 0);

    auto [it, ok] = s.insert(42);
    CHECK(ok);
    CHECK(*it == 42);
    CHECK(s.size() == 1);

    auto [it2, ok2] = s.insert(42); // duplicate
    CHECK(!ok2);
    CHECK(*it2 == 42);
    CHECK(s.size() == 1);

    s.insert(1); s.insert(2); s.insert(3);
    CHECK(s.size() == 4);
}

// ---- test_set_iteration ----
void test_set_iteration() {
    std::set<int> s = {5, 3, 1, 4, 2};
    int prev = -1;
    for (int v : s) {
        CHECK(v > prev);
        prev = v;
    }
    CHECK(prev == 5);
}

// ---- test_set_erase ----
void test_set_erase() {
    std::set<int> s;
    for (int i = 0; i < 10; ++i) s.insert(i);

    auto n = s.erase(5);
    CHECK(n == 1);
    CHECK(!s.contains(5));
    CHECK(s.size() == 9);

    n = s.erase(99);
    CHECK(n == 0);

    auto it = s.find(3);
    s.erase(it);
    CHECK(!s.contains(3));
    CHECK(s.size() == 8);

    auto lo = s.lower_bound(6);
    auto hi = s.upper_bound(8);
    s.erase(lo, hi);
    CHECK(!s.contains(6));
    CHECK(!s.contains(7));
    CHECK(!s.contains(8));
}

// ---- test_set_find_count_contains ----
void test_set_find_count_contains() {
    std::set<int> s = {1, 2, 3, 4, 5};

    CHECK(s.find(3) != s.end());
    CHECK(*s.find(3) == 3);
    CHECK(s.find(99) == s.end());

    CHECK(s.count(1) == 1);
    CHECK(s.count(99) == 0);

    CHECK(s.contains(5));
    CHECK(!s.contains(0));
}

// ---- test_set_bounds ----
void test_set_bounds() {
    std::set<int> s = {1, 2, 3, 4, 5};

    CHECK(*s.lower_bound(3) == 3);
    CHECK(*s.upper_bound(3) == 4);
    CHECK(*s.lower_bound(0) == 1);
    CHECK(s.upper_bound(5) == s.end());

    auto [lo, hi] = s.equal_range(3);
    CHECK(*lo == 3);
    CHECK(*hi == 4);
}

// ---- test_set_emplace ----
void test_set_emplace() {
    std::set<int> s;
    auto [it, ok] = s.emplace(10);
    CHECK(ok);
    CHECK(*it == 10);

    auto [it2, ok2] = s.emplace(10);
    CHECK(!ok2);

    auto it3 = s.emplace_hint(s.end(), 20);
    CHECK(*it3 == 20);
}

// ---- test_set_copy_move ----
void test_set_copy_move() {
    std::set<int> s = {1, 2, 3};

    std::set<int> copy(s);
    CHECK(copy.size() == 3);
    CHECK(copy.contains(2));

    std::set<int> moved(std::move(s));
    CHECK(moved.size() == 3);
    CHECK(s.empty());

    std::set<int> a;
    a = copy;
    CHECK(a.size() == 3);

    std::set<int> b;
    b = std::move(copy);
    CHECK(b.size() == 3);
    CHECK(copy.empty());
}

// ---- test_set_swap ----
void test_set_swap() {
    std::set<int> a = {1, 2};
    std::set<int> b = {10, 20, 30};

    a.swap(b);
    CHECK(a.size() == 3);
    CHECK(b.size() == 2);
    CHECK(a.contains(10));
    CHECK(b.contains(1));

    std::swap(a, b);
    CHECK(a.size() == 2);
    CHECK(b.size() == 3);
}

// ---- test_set_clear ----
void test_set_clear() {
    std::set<int> s;
    for (int i = 0; i < 20; ++i) s.insert(i);
    s.clear();
    CHECK(s.empty());
    CHECK(s.begin() == s.end());
}

// ---- test_set_compare ----
void test_set_compare() {
    std::set<int> a = {1, 2, 3};
    std::set<int> b = {1, 2, 3};
    std::set<int> c = {1, 2, 4};

    CHECK(a == b);
    CHECK(!(a == c));
    CHECK((a <=> b) == 0);
    CHECK((a <=> c) < 0);
}

// ---- test_set_node_handle ----
void test_set_node_handle() {
    std::set<int> s = {1, 2, 3};

    auto nh = s.extract(2);
    CHECK(bool(nh));
    CHECK(nh.value() == 2);
    CHECK(s.size() == 2);
    CHECK(!s.contains(2));

    auto [it, inserted, nh2] = s.insert(std::move(nh));
    CHECK(inserted);
    CHECK(*it == 2);
    CHECK(s.size() == 3);

    auto nh3 = s.extract(99);
    CHECK(nh3.empty());
}

// ---- test_set_merge ----
void test_set_merge() {
    std::set<int> a = {1, 2, 3};
    std::set<int> b = {2, 4, 5};

    a.merge(b);
    CHECK(a.size() == 5);
    CHECK(a.contains(4));
    CHECK(b.size() == 1); // 2 stayed in b
    CHECK(b.contains(2));
}

// ---- test_set_reverse_iteration ----
void test_set_reverse_iteration() {
    std::set<int> s = {1, 2, 3, 4, 5};
    int prev = 999;
    for (auto it = s.rbegin(); it != s.rend(); ++it) {
        CHECK(*it < prev);
        prev = *it;
    }
    CHECK(prev == 1);
}

// ---- test_set_erase_if ----
void test_set_erase_if() {
    std::set<int> s = {1, 2, 3, 4, 5, 6};
    auto n = std::erase_if(s, [](int v) { return v % 2 == 0; });
    CHECK(n == 3);
    CHECK(s.size() == 3);
    CHECK(!s.contains(2));
    CHECK(!s.contains(4));
    CHECK(!s.contains(6));
    CHECK(s.contains(1));
}

// ---- test_set_initializer_list ----
void test_set_initializer_list() {
    std::set<int> s = {5, 3, 1, 4, 2};
    CHECK(s.size() == 5);
    s = {10, 20};
    CHECK(s.size() == 2);
    CHECK(s.contains(10));
}

// ---- test_set_large ----
void test_set_large() {
    std::set<int> s;
    for (int i = 0; i < 1000; ++i) s.insert(i);
    CHECK(s.size() == 1000);
    for (int i = 0; i < 500; ++i) s.erase(i * 2);
    CHECK(s.size() == 500);
    int prev = -1;
    for (int v : s) {
        CHECK(v > prev);
        prev = v;
    }
}

// ---- test_multiset_basic ----
void test_multiset_basic() {
    std::multiset<int> s;
    s.insert(1); s.insert(1); s.insert(1);
    s.insert(2);
    CHECK(s.size() == 4);
    CHECK(s.count(1) == 3);
    CHECK(s.count(2) == 1);
}

// ---- test_multiset_equal_range ----
void test_multiset_equal_range() {
    std::multiset<int> s = {1, 1, 1, 2, 3};
    auto [lo, hi] = s.equal_range(1);
    int cnt = 0;
    for (auto it = lo; it != hi; ++it) ++cnt;
    CHECK(cnt == 3);
}

// ---- test_multiset_erase ----
void test_multiset_erase() {
    std::multiset<int> s = {1, 1, 1, 2, 2, 3};
    auto n = s.erase(1);
    CHECK(n == 3);
    CHECK(s.size() == 3);
    CHECK(!s.contains(1));
}

// ---- test_multiset_node_handle ----
void test_multiset_node_handle() {
    std::multiset<int> s = {1, 1, 2};
    auto nh = s.extract(s.find(1));
    CHECK(bool(nh));
    CHECK(nh.value() == 1);
    CHECK(s.size() == 2);
    CHECK(s.count(1) == 1);
}

// ---- test_multiset_merge ----
void test_multiset_merge() {
    std::multiset<int> a = {1, 2, 3};
    std::multiset<int> b = {2, 4, 5};
    a.merge(b);
    CHECK(a.size() == 6);
    CHECK(b.empty());
}

// ---- test_set_merge_from_multiset ----
void test_set_merge_from_multiset() {
    std::multiset<int> ms = {1, 2, 2, 3};
    std::set<int> s = {5};
    s.merge(ms);
    CHECK(s.contains(1));
    CHECK(s.contains(2));
    CHECK(s.contains(3));
    // one copy of 2 went to s, the duplicate stayed in ms
    CHECK(ms.count(2) == 1);
}
