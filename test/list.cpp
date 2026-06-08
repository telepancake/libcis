// Tests for <list>
// Each test is a free function void test_list_<case>().
// No progress printing; CHECK traps on failure.
#include <list>
#include "test.h"

// ============================================================
void test_list_default_ctor() {
    std::list<int> l;
    CHECK(l.empty());
    CHECK(l.size() == 0);
    CHECK(l.begin() == l.end());
}

void test_list_size_ctor() {
    std::list<int> l(5);
    CHECK(l.size() == 5);
    for (auto& v : l) CHECK(v == 0);
}

void test_list_size_value_ctor() {
    std::list<int> l(3, 42);
    CHECK(l.size() == 3);
    for (auto& v : l) CHECK(v == 42);
}

void test_list_iter_ctor() {
    int arr[] = {1, 2, 3, 4, 5};
    std::list<int> l(arr, arr + 5);
    CHECK(l.size() == 5);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
}

void test_list_copy_ctor() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b(a);
    CHECK(b.size() == 3);
    CHECK(a == b);
}

void test_list_move_ctor() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b(std::move(a));
    CHECK(b.size() == 3);
    CHECK(a.empty());
    int i = 1;
    for (auto& v : b) CHECK(v == i++);
}

void test_list_initializer_list_ctor() {
    std::list<int> l = {10, 20, 30};
    CHECK(l.size() == 3);
    CHECK(l.front() == 10);
    CHECK(l.back() == 30);
}

void test_list_copy_assign() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b = {4, 5};
    b = a;
    CHECK(b.size() == 3);
    CHECK(a == b);
}

void test_list_move_assign() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b;
    b = std::move(a);
    CHECK(b.size() == 3);
    CHECK(a.empty());
}

void test_list_assign_range() {
    std::list<int> l;
    int arr[] = {7, 8, 9};
    l.assign(arr, arr + 3);
    CHECK(l.size() == 3);
    CHECK(l.front() == 7);
    CHECK(l.back() == 9);
}

void test_list_assign_n_val() {
    std::list<int> l = {1, 2, 3, 4};
    l.assign(2, 99);
    CHECK(l.size() == 2);
    for (auto& v : l) CHECK(v == 99);
}

void test_list_assign_ilist() {
    std::list<int> l;
    l.assign({1, 2, 3});
    CHECK(l.size() == 3);
    CHECK(l.front() == 1);
}

void test_list_front_back() {
    std::list<int> l = {1, 2, 3};
    CHECK(l.front() == 1);
    CHECK(l.back() == 3);
    l.front() = 10;
    l.back()  = 30;
    CHECK(l.front() == 10);
    CHECK(l.back() == 30);
}

void test_list_iterators() {
    std::list<int> l = {1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = l.begin(); it != l.end(); ++it) sum += *it;
    CHECK(sum == 15);

    sum = 0;
    for (auto it = l.cbegin(); it != l.cend(); ++it) sum += *it;
    CHECK(sum == 15);

    // reverse iteration
    sum = 0;
    for (auto it = l.rbegin(); it != l.rend(); ++it) sum += *it;
    CHECK(sum == 15);
}

void test_list_push_pop_front() {
    std::list<int> l;
    l.push_front(2);
    l.push_front(1);
    CHECK(l.front() == 1);
    CHECK(l.size() == 2);
    l.pop_front();
    CHECK(l.front() == 2);
    CHECK(l.size() == 1);
}

void test_list_push_pop_back() {
    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    CHECK(l.back() == 2);
    CHECK(l.size() == 2);
    l.pop_back();
    CHECK(l.back() == 1);
    CHECK(l.size() == 1);
}

void test_list_emplace() {
    std::list<std::pair<int,int>> l;
    l.emplace_back(1, 2);
    l.emplace_front(0, 0);
    auto it = l.end();
    l.emplace(it, 3, 4);
    CHECK(l.size() == 3);
    CHECK(l.front().first == 0);
    CHECK(l.back().first == 3);
}

void test_list_insert_single() {
    std::list<int> l = {1, 3};
    auto it = l.begin();
    ++it;
    auto r = l.insert(it, 2);
    CHECK(*r == 2);
    CHECK(l.size() == 3);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
}

void test_list_insert_n() {
    std::list<int> l = {1, 4};
    auto it = l.begin(); ++it;
    l.insert(it, 2, 99);
    CHECK(l.size() == 4);
}

void test_list_insert_range() {
    std::list<int> l = {1, 5};
    int arr[] = {2, 3, 4};
    auto it = l.begin(); ++it;
    l.insert(it, arr, arr + 3);
    CHECK(l.size() == 5);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
}

void test_list_insert_ilist() {
    std::list<int> l = {1, 5};
    auto it = l.begin(); ++it;
    l.insert(it, {2, 3, 4});
    CHECK(l.size() == 5);
}

void test_list_erase_single() {
    std::list<int> l = {1, 2, 3};
    auto it = l.begin(); ++it;
    auto r = l.erase(it);
    CHECK(*r == 3);
    CHECK(l.size() == 2);
    CHECK(l.front() == 1);
    CHECK(l.back() == 3);
}

void test_list_erase_range() {
    std::list<int> l = {1, 2, 3, 4, 5};
    auto f = l.begin(); ++f;
    auto la = f; ++la; ++la; ++la;
    l.erase(f, la);
    CHECK(l.size() == 2);
    CHECK(l.front() == 1);
    CHECK(l.back() == 5);
}

void test_list_resize() {
    std::list<int> l = {1, 2, 3};
    l.resize(5, 0);
    CHECK(l.size() == 5);
    l.resize(2);
    CHECK(l.size() == 2);
    CHECK(l.front() == 1);
    CHECK(l.back() == 2);
}

void test_list_clear() {
    std::list<int> l = {1, 2, 3};
    l.clear();
    CHECK(l.empty());
    CHECK(l.size() == 0);
}

void test_list_swap() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b = {4, 5};
    a.swap(b);
    CHECK(a.size() == 2);
    CHECK(b.size() == 3);
    CHECK(a.front() == 4);
    CHECK(b.front() == 1);
}

void test_list_nonmember_swap() {
    std::list<int> a = {1, 2};
    std::list<int> b = {3, 4, 5};
    std::swap(a, b);
    CHECK(a.size() == 3);
    CHECK(b.size() == 2);
}

void test_list_splice_whole() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b = {4, 5, 6};
    a.splice(a.end(), b);
    CHECK(a.size() == 6);
    CHECK(b.empty());
    CHECK(a.back() == 6);
}

void test_list_splice_single() {
    std::list<int> a = {1, 3};
    std::list<int> b = {2};
    a.splice(std::next(a.begin()), b, b.begin());
    CHECK(a.size() == 3);
    CHECK(b.empty());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
}

void test_list_splice_range() {
    std::list<int> a = {1, 5};
    std::list<int> b = {2, 3, 4};
    a.splice(std::next(a.begin()), b, b.begin(), b.end());
    CHECK(a.size() == 5);
    CHECK(b.empty());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
}

void test_list_remove() {
    std::list<int> l = {1, 2, 3, 2, 4, 2};
    auto n = l.remove(2);
    CHECK(n == 3);
    CHECK(l.size() == 3);
    for (auto& v : l) CHECK(v != 2);
}

void test_list_remove_if() {
    std::list<int> l = {1, 2, 3, 4, 5, 6};
    auto n = l.remove_if([](int x){ return x % 2 == 0; });
    CHECK(n == 3);
    CHECK(l.size() == 3);
    for (auto& v : l) CHECK(v % 2 == 1);
}

void test_list_unique() {
    std::list<int> l = {1, 1, 2, 3, 3, 3, 4};
    auto n = l.unique();
    CHECK(n == 3);
    CHECK(l.size() == 4);
    int vals[] = {1, 2, 3, 4};
    int i = 0;
    for (auto& v : l) CHECK(v == vals[i++]);
}

void test_list_merge() {
    std::list<int> a = {1, 3, 5};
    std::list<int> b = {2, 4, 6};
    a.merge(b);
    CHECK(a.size() == 6);
    CHECK(b.empty());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
}

void test_list_sort() {
    std::list<int> l = {5, 3, 1, 4, 2};
    l.sort();
    CHECK(l.size() == 5);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
}

void test_list_sort_comp() {
    std::list<int> l = {1, 2, 3, 4, 5};
    l.sort(std::greater<int>{});
    int i = 5;
    for (auto& v : l) CHECK(v == i--);
}

void test_list_reverse() {
    std::list<int> l = {1, 2, 3, 4, 5};
    l.reverse();
    int i = 5;
    for (auto& v : l) CHECK(v == i--);
}

void test_list_comparisons() {
    std::list<int> a = {1, 2, 3};
    std::list<int> b = {1, 2, 3};
    std::list<int> c = {1, 2, 4};
    CHECK(a == b);
    CHECK(!(a == c));
    auto r = (a <=> c);
    CHECK(r < 0);
    CHECK((c <=> a) > 0);
    CHECK((a <=> b) == 0);
}

void test_list_erase_free() {
    std::list<int> l = {1, 2, 3, 2, 4};
    auto n = std::erase(l, 2);
    CHECK(n == 2);
    CHECK(l.size() == 3);
}

void test_list_erase_if_free() {
    std::list<int> l = {1, 2, 3, 4, 5};
    auto n = std::erase_if(l, [](int x){ return x > 3; });
    CHECK(n == 2);
    CHECK(l.size() == 3);
}

void test_list_move_only() {
    // Test with move-only type
    struct MoveOnly {
        int val;
        MoveOnly(int v) : val(v) {}
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;
    };
    std::list<MoveOnly> l;
    l.emplace_back(1);
    l.emplace_back(2);
    l.emplace_front(0);
    CHECK(l.size() == 3);
    CHECK(l.front().val == 0);
    CHECK(l.back().val == 2);
}

void test_list_splice_self() {
    // splice within the same list
    std::list<int> l = {1, 2, 3, 4, 5};
    auto first = std::next(l.begin(), 2); // points to 3
    auto last  = l.end();
    // splice [3,4,5] before begin → should result in 3,4,5,1,2
    l.splice(l.begin(), l, first, last);
    CHECK(l.size() == 5);
    int vals[] = {3, 4, 5, 1, 2};
    int i = 0;
    for (auto& v : l) CHECK(v == vals[i++]);
}

void test_list_deduction_guide() {
    int arr[] = {1, 2, 3};
    std::list l(arr, arr + 3);
    CHECK(l.size() == 3);
}
