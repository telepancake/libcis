// Tests for <unordered_set>
#include <unordered_set>
#include "../test/test.h"

void test_unordered_set_default_ctor() {
    std::unordered_set<int> s;
    CHECK(s.empty());
    CHECK(s.size() == 0);
    CHECK(s.begin() == s.end());
}

void test_unordered_set_insert_and_find() {
    std::unordered_set<int> s;
    auto [it, ins] = s.insert(1);
    CHECK(ins);
    CHECK(*it == 1);
    CHECK(s.size() == 1);

    auto [it2, ins2] = s.insert(1);
    CHECK(!ins2);
    CHECK(s.size() == 1);

    auto f = s.find(1);
    CHECK(f != s.end() && *f == 1);
    CHECK(s.find(999) == s.end());
}

void test_unordered_set_emplace() {
    std::unordered_set<int> s;
    auto [it, ins] = s.emplace(5);
    CHECK(ins && *it == 5);
    auto [it2, ins2] = s.emplace(5);
    CHECK(!ins2);
    CHECK(s.size() == 1);
}

void test_unordered_set_erase_by_key() {
    std::unordered_set<int> s = {1, 2, 3};
    CHECK(s.erase(2) == 1);
    CHECK(s.size() == 2);
    CHECK(s.find(2) == s.end());
    CHECK(s.erase(999) == 0);
}

void test_unordered_set_erase_by_iter() {
    std::unordered_set<int> s = {1, 2, 3};
    auto it = s.find(1);
    CHECK(it != s.end());
    s.erase(it);
    CHECK(s.size() == 2);
    CHECK(s.find(1) == s.end());
}

void test_unordered_set_count_contains() {
    std::unordered_set<int> s = {1, 2, 3};
    CHECK(s.count(1) == 1);
    CHECK(s.count(99) == 0);
    CHECK(s.contains(1));
    CHECK(!s.contains(99));
}

void test_unordered_set_equal_range() {
    std::unordered_set<int> s = {1, 2, 3};
    auto [b, e] = s.equal_range(2);
    CHECK(b != e && *b == 2);
    ++b;
    CHECK(b == e);
    auto [b2, e2] = s.equal_range(99);
    CHECK(b2 == e2);
}

void test_unordered_set_copy() {
    std::unordered_set<int> s = {1, 2, 3};
    std::unordered_set<int> s2(s);
    CHECK(s2.size() == 3);
    CHECK(s2.contains(1) && s2.contains(2) && s2.contains(3));
}

void test_unordered_set_move() {
    std::unordered_set<int> s = {1, 2, 3};
    std::unordered_set<int> s2(std::move(s));
    CHECK(s2.size() == 3);
    CHECK(s.empty());
}

void test_unordered_set_assign() {
    std::unordered_set<int> s = {1};
    std::unordered_set<int> s2;
    s2 = s;
    CHECK(s2.size() == 1);
    s2 = {10, 20, 30};
    CHECK(s2.size() == 3);
}

void test_unordered_set_initializer_list() {
    std::unordered_set<int> s{1, 2, 3, 4, 5};
    CHECK(s.size() == 5);
    for (int i = 1; i <= 5; ++i)
        CHECK(s.contains(i));
}

void test_unordered_set_bucket_interface() {
    std::unordered_set<int> s;
    s.reserve(16);
    s.insert(42);
    CHECK(s.bucket_count() >= 16);
    size_t b = s.bucket(42);
    CHECK(b < s.bucket_count());
    bool found = false;
    for (auto it = s.begin(b); it != s.end(b); ++it)
        if (*it == 42) found = true;
    CHECK(found);
}

void test_unordered_set_load_factor() {
    std::unordered_set<int> s;
    CHECK(s.load_factor() == 0.f);
    for (int i = 0; i < 100; ++i) s.insert(i);
    CHECK(s.load_factor() <= s.max_load_factor());
}

void test_unordered_set_rehash_reserve() {
    std::unordered_set<int> s;
    s.rehash(50);
    CHECK(s.bucket_count() >= 50);
    s.reserve(200);
    CHECK(s.bucket_count() >= 200);
}

void test_unordered_set_swap() {
    std::unordered_set<int> a = {1, 2};
    std::unordered_set<int> b = {3, 4, 5};
    a.swap(b);
    CHECK(a.size() == 3 && b.size() == 2);
    CHECK(a.contains(3) && b.contains(1));
}

void test_unordered_set_equality() {
    std::unordered_set<int> a = {1, 2, 3};
    std::unordered_set<int> b = {3, 2, 1};
    std::unordered_set<int> c = {1, 2, 4};
    CHECK(a == b);
    CHECK(!(a == c));
}

void test_unordered_set_large() {
    std::unordered_set<int> s;
    for (int i = 0; i < 1000; ++i) s.insert(i);
    CHECK(s.size() == 1000);
    for (int i = 0; i < 1000; ++i) CHECK(s.contains(i));
    for (int i = 0; i < 500; ++i) s.erase(i);
    CHECK(s.size() == 500);
    for (int i = 0; i < 500; ++i) CHECK(!s.contains(i));
    for (int i = 500; i < 1000; ++i) CHECK(s.contains(i));
}

void test_unordered_set_erase_if() {
    std::unordered_set<int> s = {1, 2, 3, 4, 5, 6};
    auto n = std::erase_if(s, [](int v){ return v % 2 == 0; });
    CHECK(n == 3);
    CHECK(s.size() == 3);
    CHECK(s.contains(1) && s.contains(3) && s.contains(5));
}

void test_unordered_set_insert_range() {
    std::unordered_set<int> s;
    int arr[] = {1, 2, 3, 4, 5};
    s.insert(arr, arr + 5);
    CHECK(s.size() == 5);
}

void test_unordered_set_clear() {
    std::unordered_set<int> s = {1, 2, 3};
    s.clear();
    CHECK(s.empty());
    CHECK(s.size() == 0);
}

// ============================================================================
// unordered_multiset tests
// ============================================================================

void test_unordered_multiset_basic() {
    std::unordered_multiset<int> s;
    s.insert(1);
    s.insert(1);
    s.insert(2);
    CHECK(s.size() == 3);
    CHECK(s.count(1) == 2);
    CHECK(s.count(2) == 1);
    CHECK(s.count(99) == 0);
}

void test_unordered_multiset_emplace() {
    std::unordered_multiset<int> s;
    s.emplace(5);
    s.emplace(5);
    s.emplace(5);
    CHECK(s.size() == 3);
    CHECK(s.count(5) == 3);
}

void test_unordered_multiset_erase() {
    std::unordered_multiset<int> s;
    s.insert(1); s.insert(1); s.insert(2);
    auto n = s.erase(1);
    CHECK(n == 2);
    CHECK(s.size() == 1);
    CHECK(s.count(1) == 0);
}

void test_unordered_multiset_equal_range() {
    std::unordered_multiset<int> s;
    s.insert(1); s.insert(1); s.insert(2);
    auto [b, e] = s.equal_range(1);
    size_t cnt = 0;
    for (auto it = b; it != e; ++it) ++cnt;
    CHECK(cnt == 2);
}

void test_unordered_multiset_copy() {
    std::unordered_multiset<int> s;
    s.insert(1); s.insert(1); s.insert(2);
    std::unordered_multiset<int> s2(s);
    CHECK(s2.size() == 3);
    CHECK(s2.count(1) == 2);
}

void test_unordered_multiset_equality() {
    std::unordered_multiset<int> a, b;
    a.insert(1); a.insert(1); a.insert(2);
    b.insert(2); b.insert(1); b.insert(1);
    CHECK(a == b);
    std::unordered_multiset<int> c;
    c.insert(1); c.insert(2); c.insert(2);
    CHECK(!(a == c));
}

void test_unordered_multiset_swap() {
    std::unordered_multiset<int> a = {1, 1, 2};
    std::unordered_multiset<int> b = {3, 4};
    a.swap(b);
    CHECK(a.size() == 2 && b.size() == 3);
}

void test_unordered_multiset_contains() {
    std::unordered_multiset<int> s = {1, 2, 3};
    CHECK(s.contains(1));
    CHECK(!s.contains(99));
}

void test_unordered_multiset_erase_if() {
    std::unordered_multiset<int> s;
    s.insert(1); s.insert(2); s.insert(2); s.insert(3);
    auto n = std::erase_if(s, [](int v){ return v == 2; });
    CHECK(n == 2);
    CHECK(s.size() == 2);
    CHECK(s.count(2) == 0);
}
