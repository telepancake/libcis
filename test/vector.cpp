// Tests for <vector>
// Each test is a free function void test_<name>() — no arguments, no main().
// Failures crash via CHECK() / __builtin_trap().
#include <vector>
#include "../test/test.h"

// -----------------------------------------------------------------------
// vector<int> basic operations
// -----------------------------------------------------------------------

void test_vector_default_ctor() {
    std::vector<int> v;
    CHECK(v.empty());
    CHECK(v.size() == 0);
    CHECK(v.capacity() == 0);
    CHECK(v.begin() == v.end());
}

void test_vector_push_back() {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    CHECK(v.size() == 3);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
    CHECK(v[2] == 3);
    CHECK(v.front() == 1);
    CHECK(v.back()  == 3);
}

void test_vector_pop_back() {
    std::vector<int> v = {10, 20, 30};
    v.pop_back();
    CHECK(v.size() == 2);
    CHECK(v.back() == 20);
}

void test_vector_count_ctor() {
    std::vector<int> v(5, 42);
    CHECK(v.size() == 5);
    for (int i = 0; i < 5; ++i)
        CHECK(v[i] == 42);
}

void test_vector_size_ctor() {
    std::vector<int> v(4);
    CHECK(v.size() == 4);
    // default-constructed ints are 0
    for (size_t i = 0; i < v.size(); ++i)
        CHECK(v[i] == 0);
}

void test_vector_initializer_list() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    CHECK(v.size() == 5);
    for (int i = 0; i < 5; ++i)
        CHECK(v[i] == i + 1);
}

void test_vector_copy_ctor() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b(a);
    CHECK(b.size() == 3);
    CHECK(b[0] == 1 && b[1] == 2 && b[2] == 3);
    // mutating b doesn't affect a
    b[0] = 99;
    CHECK(a[0] == 1);
}

void test_vector_move_ctor() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b(std::move(a));
    CHECK(b.size() == 3);
    CHECK(a.empty());
    CHECK(b[0] == 1 && b[1] == 2 && b[2] == 3);
}

void test_vector_copy_assign() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b;
    b = a;
    CHECK(b.size() == 3);
    CHECK(b[0] == 1);
    b[0] = 99;
    CHECK(a[0] == 1); // independent copy
}

void test_vector_move_assign() {
    std::vector<int> a = {10, 20, 30};
    std::vector<int> b;
    b = std::move(a);
    CHECK(b.size() == 3);
    CHECK(a.empty());
    CHECK(b[0] == 10 && b[1] == 20 && b[2] == 30);
}

void test_vector_assign_il() {
    std::vector<int> v;
    v = {7, 8, 9};
    CHECK(v.size() == 3 && v[0] == 7 && v[2] == 9);
}

void test_vector_at() {
    std::vector<int> v = {1, 2, 3};
    CHECK(v.at(0) == 1);
    CHECK(v.at(2) == 3);
}

void test_vector_reserve() {
    std::vector<int> v;
    v.reserve(100);
    CHECK(v.capacity() >= 100);
    CHECK(v.empty());
}

void test_vector_shrink_to_fit() {
    std::vector<int> v(10, 1);
    v.resize(3);
    v.shrink_to_fit();
    CHECK(v.size() == 3);
    // capacity may or may not be 3; just check it compiled and ran
}

void test_vector_resize() {
    std::vector<int> v = {1, 2, 3};
    v.resize(5, 99);
    CHECK(v.size() == 5);
    CHECK(v[3] == 99 && v[4] == 99);
    v.resize(2);
    CHECK(v.size() == 2);
    CHECK(v[0] == 1 && v[1] == 2);
}

void test_vector_erase_pos() {
    std::vector<int> v = {10, 20, 30, 40};
    auto it = v.erase(v.begin() + 1);
    CHECK(v.size() == 3);
    CHECK(v[0] == 10 && v[1] == 30 && v[2] == 40);
    CHECK(*it == 30);
}

void test_vector_erase_range() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 1, v.begin() + 3);
    CHECK(v.size() == 3);
    CHECK(v[0] == 1 && v[1] == 4 && v[2] == 5);
    CHECK(*it == 4);
}

void test_vector_insert_pos() {
    std::vector<int> v = {1, 2, 3};
    auto it = v.insert(v.begin() + 1, 99);
    CHECK(v.size() == 4);
    CHECK(v[0] == 1 && v[1] == 99 && v[2] == 2);
    CHECK(*it == 99);
}

void test_vector_insert_count() {
    std::vector<int> v = {1, 2, 3};
    v.insert(v.begin() + 1, 2, 55);
    CHECK(v.size() == 5);
    CHECK(v[1] == 55 && v[2] == 55 && v[3] == 2);
}

void test_vector_insert_range() {
    std::vector<int> a = {10, 20, 30};
    std::vector<int> b = {1, 2, 3};
    b.insert(b.begin() + 1, a.begin(), a.end());
    CHECK(b.size() == 6);
    CHECK(b[0] == 1 && b[1] == 10 && b[2] == 20 && b[3] == 30 && b[4] == 2);
}

void test_vector_insert_il() {
    std::vector<int> v = {1, 4, 5};
    v.insert(v.begin() + 1, {2, 3});
    CHECK(v.size() == 5);
    for (int i = 0; i < 5; ++i)
        CHECK(v[i] == i + 1);
}

void test_vector_emplace_back() {
    std::vector<int> v;
    int& r = v.emplace_back(42);
    CHECK(r == 42);
    CHECK(v.size() == 1);
}

void test_vector_emplace() {
    std::vector<int> v = {1, 3};
    auto it = v.emplace(v.begin() + 1, 2);
    CHECK(v.size() == 3);
    CHECK(*it == 2);
    CHECK(v[0] == 1 && v[1] == 2 && v[2] == 3);
}

void test_vector_clear() {
    std::vector<int> v = {1, 2, 3};
    v.clear();
    CHECK(v.empty());
    CHECK(v.size() == 0);
}

void test_vector_swap() {
    std::vector<int> a = {1, 2};
    std::vector<int> b = {3, 4, 5};
    a.swap(b);
    CHECK(a.size() == 3 && a[0] == 3);
    CHECK(b.size() == 2 && b[0] == 1);
}

void test_vector_swap_free() {
    std::vector<int> a = {1, 2};
    std::vector<int> b = {3};
    std::swap(a, b);
    CHECK(a.size() == 1 && a[0] == 3);
    CHECK(b.size() == 2);
}

void test_vector_comparison() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2, 3};
    std::vector<int> c = {1, 2, 4};
    CHECK(a == b);
    CHECK(!(a == c));
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b);
    CHECK(a >= b);
}

void test_vector_spaceship() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2, 4};
    auto r = a <=> b;
    CHECK(r < 0);
}

void test_vector_iterators() {
    std::vector<int> v = {10, 20, 30};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;
    CHECK(sum == 60);

    sum = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        sum += *it;
    CHECK(sum == 60);
}

void test_vector_data() {
    std::vector<int> v = {5, 6, 7};
    int* p = v.data();
    CHECK(p[0] == 5 && p[1] == 6 && p[2] == 7);
}

void test_vector_max_size() {
    std::vector<int> v;
    CHECK(v.max_size() > 0);
}

void test_vector_assign_count() {
    std::vector<int> v = {1, 2, 3};
    v.assign(5, 0);
    CHECK(v.size() == 5);
    for (auto x : v) CHECK(x == 0);
    v.assign(2, 9);
    CHECK(v.size() == 2 && v[0] == 9 && v[1] == 9);
}

void test_vector_assign_range() {
    std::vector<int> src = {10, 20, 30};
    std::vector<int> v;
    v.assign(src.begin(), src.end());
    CHECK(v.size() == 3 && v[0] == 10 && v[2] == 30);
}

void test_vector_erase_free() {
    std::vector<int> v = {1, 2, 3, 2, 4};
    auto n = std::erase(v, 2);
    CHECK(n == 2);
    CHECK(v.size() == 3);
    CHECK(v[0] == 1 && v[1] == 3 && v[2] == 4);
}

void test_vector_erase_if_free() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto n = std::erase_if(v, [](int x){ return x % 2 == 0; });
    CHECK(n == 2);
    CHECK(v.size() == 3);
    CHECK(v[0] == 1 && v[1] == 3 && v[2] == 5);
}

// -----------------------------------------------------------------------
// vector<bool>
// -----------------------------------------------------------------------

void test_vector_bool_default() {
    std::vector<bool> v;
    CHECK(v.empty());
    CHECK(v.size() == 0);
}

void test_vector_bool_push_back() {
    std::vector<bool> v;
    v.push_back(true);
    v.push_back(false);
    v.push_back(true);
    CHECK(v.size() == 3);
    CHECK(bool(v[0]) == true);
    CHECK(bool(v[1]) == false);
    CHECK(bool(v[2]) == true);
}

void test_vector_bool_count_ctor() {
    std::vector<bool> v(5, true);
    CHECK(v.size() == 5);
    for (size_t i = 0; i < 5; ++i)
        CHECK(bool(v[i]) == true);
}

void test_vector_bool_zero_ctor() {
    std::vector<bool> v(8);
    CHECK(v.size() == 8);
    for (size_t i = 0; i < 8; ++i)
        CHECK(bool(v[i]) == false);
}

void test_vector_bool_initializer_list() {
    std::vector<bool> v = {true, false, true, true, false};
    CHECK(v.size() == 5);
    CHECK(bool(v[0]) == true);
    CHECK(bool(v[1]) == false);
    CHECK(bool(v[4]) == false);
}

void test_vector_bool_copy() {
    std::vector<bool> a = {true, false, true};
    std::vector<bool> b(a);
    CHECK(b.size() == 3);
    CHECK(bool(b[0]) && !bool(b[1]) && bool(b[2]));
    b[0] = false;
    CHECK(bool(a[0]) == true); // independent
}

void test_vector_bool_move() {
    std::vector<bool> a = {true, false};
    std::vector<bool> b(std::move(a));
    CHECK(b.size() == 2 && a.empty());
    CHECK(bool(b[0]) == true);
}

void test_vector_bool_assign() {
    std::vector<bool> v;
    v.assign(4, true);
    CHECK(v.size() == 4);
    for (size_t i = 0; i < 4; ++i)
        CHECK(bool(v[i]) == true);
}

void test_vector_bool_resize() {
    std::vector<bool> v = {true, false, true};
    v.resize(5, true);
    CHECK(v.size() == 5);
    CHECK(bool(v[3]) == true && bool(v[4]) == true);
    v.resize(2);
    CHECK(v.size() == 2);
}

void test_vector_bool_flip() {
    std::vector<bool> v = {true, false, true, false};
    v.flip();
    CHECK(bool(v[0]) == false);
    CHECK(bool(v[1]) == true);
    CHECK(bool(v[2]) == false);
    CHECK(bool(v[3]) == true);
}

void test_vector_bool_erase() {
    std::vector<bool> v = {true, false, true, true};
    v.erase(v.begin() + 1);
    CHECK(v.size() == 3);
    CHECK(bool(v[0]) == true && bool(v[1]) == true && bool(v[2]) == true);
}

void test_vector_bool_erase_range() {
    std::vector<bool> v = {true, false, false, true};
    v.erase(v.begin() + 1, v.begin() + 3);
    CHECK(v.size() == 2);
    CHECK(bool(v[0]) == true && bool(v[1]) == true);
}

void test_vector_bool_insert() {
    std::vector<bool> v = {true, true};
    v.insert(v.begin() + 1, false);
    CHECK(v.size() == 3);
    CHECK(bool(v[0]) == true && bool(v[1]) == false && bool(v[2]) == true);
}

void test_vector_bool_insert_count() {
    std::vector<bool> v = {true, true};
    v.insert(v.begin() + 1, 2, false);
    CHECK(v.size() == 4);
    CHECK(bool(v[0]) == true);
    CHECK(bool(v[1]) == false && bool(v[2]) == false);
    CHECK(bool(v[3]) == true);
}

void test_vector_bool_swap() {
    std::vector<bool> a = {true, false};
    std::vector<bool> b = {false, false, true};
    a.swap(b);
    CHECK(a.size() == 3 && b.size() == 2);
    CHECK(bool(a[0]) == false && bool(b[0]) == true);
}

void test_vector_bool_comparison() {
    std::vector<bool> a = {true, false};
    std::vector<bool> b = {true, false};
    std::vector<bool> c = {true, true};
    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
}

void test_vector_bool_reference() {
    std::vector<bool> v = {false};
    std::vector<bool>::reference r = v[0];
    r = true;
    CHECK(bool(v[0]) == true);
    r.flip();
    CHECK(bool(v[0]) == false);
}

void test_vector_bool_front_back() {
    std::vector<bool> v = {true, false, true};
    CHECK(bool(v.front()) == true);
    CHECK(bool(v.back())  == true);
}

void test_vector_bool_large() {
    // Cross word-boundary: 100 elements
    std::vector<bool> v(100, false);
    for (int i = 0; i < 100; i += 3)
        v[i] = true;
    for (int i = 0; i < 100; ++i)
        CHECK(bool(v[i]) == (i % 3 == 0));
}

void test_vector_bool_hash() {
    std::vector<bool> v = {true, false, true};
    std::hash<std::vector<bool>> h;
    size_t hv = h(v);
    (void)hv; // just verify it compiles and runs
    std::vector<bool> v2 = v;
    CHECK(h(v) == h(v2));
}

// -----------------------------------------------------------------------
// Static (compile-time) checks
// -----------------------------------------------------------------------

static_assert(std::is_same_v<std::vector<int>::value_type, int>);
static_assert(std::is_same_v<std::vector<int>::size_type, size_t>);
static_assert(std::is_same_v<std::vector<bool>::value_type, bool>);
// std::vector<bool>::reference is an implementation-defined nested class (not std::bit_reference).
// The standard only requires it to support assignment and conversion to bool.
static_assert(!std::is_same_v<std::vector<bool>::reference, bool>); // it is a proxy, not plain bool
