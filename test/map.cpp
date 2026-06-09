// test/map.cpp — tests for <map> and <multimap>
// No #pragma once (test file)
#include <map>
#include "test.h"

// ---- test_map_basic ----
void test_map_basic() {
    std::map<int, int> m;
    CHECK(m.empty());
    CHECK(m.size() == 0);

    auto [it, ok] = m.insert({1, 10});
    CHECK(ok);
    CHECK(it->first == 1);
    CHECK(it->second == 10);
    CHECK(m.size() == 1);
    CHECK(!m.empty());

    auto [it2, ok2] = m.insert({1, 20}); // duplicate
    CHECK(!ok2);
    CHECK(it2->second == 10); // existing value unchanged

    m.insert({2, 20});
    m.insert({3, 30});
    CHECK(m.size() == 3);
}

// ---- test_map_operator_bracket ----
void test_map_operator_bracket() {
    std::map<int, int> m;
    m[1] = 100;
    m[2] = 200;
    CHECK(m[1] == 100);
    CHECK(m[2] == 200);
    CHECK(m.size() == 2);
    m[1] = 999;
    CHECK(m[1] == 999);
    // operator[] inserts default value for new key
    int v = m[5];
    CHECK(v == 0);
    CHECK(m.size() == 3);
}

// ---- test_map_at ----
void test_map_at() {
    std::map<int, int> m;
    m[1] = 42;
    CHECK(m.at(1) == 42);
    m.at(1) = 99;
    CHECK(m.at(1) == 99);
}

// ---- test_map_iteration ----
void test_map_iteration() {
    std::map<int, int> m;
    m[3] = 30; m[1] = 10; m[2] = 20;
    int prev = -1;
    int count = 0;
    for (auto& [k, v] : m) {
        CHECK(k > prev);
        prev = k;
        ++count;
    }
    CHECK(count == 3);
}

// ---- test_map_erase ----
void test_map_erase() {
    std::map<int, int> m;
    for (int i = 0; i < 10; ++i) m[i] = i * 10;
    CHECK(m.size() == 10);

    // erase by key
    auto n = m.erase(5);
    CHECK(n == 1);
    CHECK(m.size() == 9);
    CHECK(m.find(5) == m.end());

    // erase non-existent
    n = m.erase(99);
    CHECK(n == 0);

    // erase by iterator
    auto it = m.find(3);
    CHECK(it != m.end());
    m.erase(it);
    CHECK(m.find(3) == m.end());
    CHECK(m.size() == 8);

    // erase range
    auto lo = m.lower_bound(6);
    auto hi = m.upper_bound(8);
    m.erase(lo, hi);
    CHECK(m.find(6) == m.end());
    CHECK(m.find(7) == m.end());
    CHECK(m.find(8) == m.end());
}

// ---- test_map_find_count_contains ----
void test_map_find_count_contains() {
    std::map<int, int> m;
    m[1] = 1; m[2] = 2; m[3] = 3;

    CHECK(m.find(2) != m.end());
    CHECK(m.find(2)->second == 2);
    CHECK(m.find(99) == m.end());

    CHECK(m.count(1) == 1);
    CHECK(m.count(99) == 0);

    CHECK(m.contains(3));
    CHECK(!m.contains(0));
}

// ---- test_map_bounds ----
void test_map_bounds() {
    std::map<int, int> m;
    for (int i = 1; i <= 5; ++i) m[i] = i;

    CHECK(m.lower_bound(3)->first == 3);
    CHECK(m.upper_bound(3)->first == 4);
    CHECK(m.lower_bound(0)->first == 1);
    CHECK(m.upper_bound(5) == m.end());

    auto [lo, hi] = m.equal_range(3);
    CHECK(lo->first == 3);
    CHECK(hi->first == 4);
}

// ---- test_map_emplace ----
void test_map_emplace() {
    std::map<int, int> m;
    auto [it, ok] = m.emplace(1, 10);
    CHECK(ok);
    CHECK(it->second == 10);

    auto [it2, ok2] = m.emplace(1, 20);
    CHECK(!ok2);
    CHECK(it2->second == 10);

    // emplace_hint
    auto it3 = m.emplace_hint(m.end(), 2, 20);
    CHECK(it3->second == 20);
}

// ---- test_map_try_emplace ----
void test_map_try_emplace() {
    std::map<int, int> m;
    auto [it, ok] = m.try_emplace(1, 42);
    CHECK(ok);
    CHECK(it->second == 42);

    auto [it2, ok2] = m.try_emplace(1, 99);
    CHECK(!ok2);
    CHECK(it2->second == 42); // not changed

    // with rvalue key
    auto [it3, ok3] = m.try_emplace(2, 100);
    CHECK(ok3);
    CHECK(it3->second == 100);
}

// ---- test_map_insert_or_assign ----
void test_map_insert_or_assign() {
    std::map<int, int> m;
    auto [it, ok] = m.insert_or_assign(1, 10);
    CHECK(ok);
    CHECK(it->second == 10);

    auto [it2, ok2] = m.insert_or_assign(1, 99);
    CHECK(!ok2);
    CHECK(it2->second == 99); // assigned

    auto it3 = m.insert_or_assign(m.end(), 2, 20);
    CHECK(it3->second == 20);
}

// ---- test_map_copy_move ----
void test_map_copy_move() {
    std::map<int, int> m;
    m[1] = 1; m[2] = 2; m[3] = 3;

    std::map<int, int> copy(m);
    CHECK(copy.size() == 3);
    CHECK(copy[1] == 1);

    std::map<int, int> moved(std::move(m));
    CHECK(moved.size() == 3);
    CHECK(m.empty());

    std::map<int, int> a;
    a = copy;
    CHECK(a.size() == 3);

    std::map<int, int> b;
    b = std::move(copy);
    CHECK(b.size() == 3);
    CHECK(copy.empty());
}

// ---- test_map_clear ----
void test_map_clear() {
    std::map<int, int> m;
    for (int i = 0; i < 20; ++i) m[i] = i;
    CHECK(m.size() == 20);
    m.clear();
    CHECK(m.empty());
    CHECK(m.size() == 0);
    CHECK(m.begin() == m.end());
}

// ---- test_map_swap ----
void test_map_swap() {
    std::map<int, int> a, b;
    a[1] = 1; a[2] = 2;
    b[10] = 10; b[20] = 20; b[30] = 30;

    a.swap(b);
    CHECK(a.size() == 3);
    CHECK(b.size() == 2);
    CHECK(a.contains(10));
    CHECK(b.contains(1));

    std::swap(a, b);
    CHECK(a.size() == 2);
    CHECK(b.size() == 3);
}

// ---- test_map_initializer_list ----
void test_map_initializer_list() {
    std::map<int, int> m = {{1,10}, {2,20}, {3,30}};
    CHECK(m.size() == 3);
    CHECK(m[2] == 20);

    m = {{5,50}, {6,60}};
    CHECK(m.size() == 2);
    CHECK(m[5] == 50);
}

// ---- test_map_compare ----
void test_map_compare() {
    std::map<int, int> a = {{1,1},{2,2}};
    std::map<int, int> b = {{1,1},{2,2}};
    std::map<int, int> c = {{1,1},{3,3}};

    CHECK(a == b);
    CHECK(!(a == c));
    CHECK((a <=> b) == 0);
    CHECK((a <=> c) < 0);
}

// ---- test_map_node_handle ----
void test_map_node_handle() {
    std::map<int, int> m;
    m[1] = 10; m[2] = 20; m[3] = 30;

    // extract by key
    auto nh = m.extract(2);
    CHECK(bool(nh));
    CHECK(nh.key() == 2);
    CHECK(nh.mapped() == 20);
    CHECK(m.size() == 2);
    CHECK(!m.contains(2));

    // re-insert
    auto [it, inserted, nh2] = m.insert(std::move(nh));
    CHECK(inserted);
    CHECK(it->second == 20);
    CHECK(m.size() == 3);

    // extract non-existent
    auto nh3 = m.extract(99);
    CHECK(nh3.empty());

    // extract by iterator
    auto nh4 = m.extract(m.find(1));
    CHECK(nh4.key() == 1);
    CHECK(m.size() == 2);
}

// ---- test_map_merge ----
void test_map_merge() {
    std::map<int, int> a = {{1,1},{2,2},{3,3}};
    std::map<int, int> b = {{2,22},{4,44},{5,55}};

    a.merge(b);
    CHECK(a.size() == 5);
    CHECK(a[2] == 2);   // existing key not overwritten
    CHECK(a[4] == 44);
    CHECK(b.size() == 1); // 2 stayed in b (duplicate)
    CHECK(b.contains(2));
}

// ---- test_map_reverse_iteration ----
void test_map_reverse_iteration() {
    std::map<int, int> m = {{1,1},{2,2},{3,3},{4,4}};
    int prev = 999;
    for (auto it = m.rbegin(); it != m.rend(); ++it) {
        CHECK(it->first < prev);
        prev = it->first;
    }
}

// ---- test_map_erase_if ----
void test_map_erase_if() {
    std::map<int, int> m = {{1,1},{2,2},{3,3},{4,4},{5,5}};
    auto n = std::erase_if(m, [](const auto& p) { return p.first % 2 == 0; });
    CHECK(n == 2);
    CHECK(m.size() == 3);
    CHECK(!m.contains(2));
    CHECK(!m.contains(4));
}

// ---- test_multimap_basic ----
void test_multimap_basic() {
    std::multimap<int, int> m;
    m.insert({1, 10});
    m.insert({1, 11});
    m.insert({1, 12});
    m.insert({2, 20});
    CHECK(m.size() == 4);
    CHECK(m.count(1) == 3);
    CHECK(m.count(2) == 1);
    CHECK(m.count(99) == 0);
}

// ---- test_multimap_equal_range ----
void test_multimap_equal_range() {
    std::multimap<int, int> m;
    m.insert({1, 10}); m.insert({1, 11}); m.insert({2, 20});
    auto [lo, hi] = m.equal_range(1);
    int cnt = 0;
    for (auto it = lo; it != hi; ++it) ++cnt;
    CHECK(cnt == 2);
}

// ---- test_multimap_erase ----
void test_multimap_erase() {
    std::multimap<int, int> m;
    for (int i = 0; i < 3; ++i) m.insert({1, i});
    m.insert({2, 20});
    CHECK(m.size() == 4);
    auto n = m.erase(1);
    CHECK(n == 3);
    CHECK(m.size() == 1);
    CHECK(!m.contains(1));
}

// ---- test_multimap_node_handle ----
void test_multimap_node_handle() {
    std::multimap<int, int> m;
    m.insert({1, 10}); m.insert({1, 11});
    auto nh = m.extract(m.find(1));
    CHECK(bool(nh));
    CHECK(nh.key() == 1);
    CHECK(m.size() == 1);
}

// ---- test_map_large ----
void test_map_large() {
    std::map<int, int> m;
    for (int i = 0; i < 1000; ++i)
        m[i] = i * 2;
    CHECK(m.size() == 1000);
    for (int i = 0; i < 1000; ++i)
        CHECK(m[i] == i * 2);
    for (int i = 0; i < 500; ++i)
        m.erase(i * 2);
    CHECK(m.size() == 500);
    // verify sorted order
    int prev = -1;
    for (auto& [k,v] : m) {
        CHECK(k > prev);
        prev = k;
    }
}
