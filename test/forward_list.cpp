// Tests for <forward_list>
// Each test is a free function void test_forward_list_<case>().
// No progress printing; CHECK traps on failure.
#include <forward_list>
#include "test.h"

// ============================================================
void test_forward_list_default_ctor() {
    std::forward_list<int> l;
    CHECK(l.empty());
    CHECK(l.begin() == l.end());
}

void test_forward_list_size_ctor() {
    std::forward_list<int> l(4);
    int cnt = 0;
    for (auto& v : l) { CHECK(v == 0); ++cnt; }
    CHECK(cnt == 4);
}

void test_forward_list_size_value_ctor() {
    std::forward_list<int> l(3, 7);
    int cnt = 0;
    for (auto& v : l) { CHECK(v == 7); ++cnt; }
    CHECK(cnt == 3);
}

void test_forward_list_iter_ctor() {
    int arr[] = {1, 2, 3, 4, 5};
    std::forward_list<int> l(arr, arr + 5);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
    CHECK(i == 6);
}

void test_forward_list_copy_ctor() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b(a);
    CHECK(a == b);
}

void test_forward_list_move_ctor() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b(std::move(a));
    CHECK(a.empty());
    int i = 1;
    for (auto& v : b) CHECK(v == i++);
    CHECK(i == 4);
}

void test_forward_list_initializer_list_ctor() {
    std::forward_list<int> l = {10, 20, 30};
    CHECK(l.front() == 10);
}

void test_forward_list_copy_assign() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b = {4, 5};
    b = a;
    CHECK(a == b);
}

void test_forward_list_move_assign() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b;
    b = std::move(a);
    CHECK(a.empty());
    int i = 1;
    for (auto& v : b) CHECK(v == i++);
}

void test_forward_list_assign_range() {
    std::forward_list<int> l;
    int arr[] = {7, 8, 9};
    l.assign(arr, arr + 3);
    CHECK(l.front() == 7);
}

void test_forward_list_assign_n_val() {
    std::forward_list<int> l = {1, 2, 3, 4};
    l.assign(2, 99);
    int cnt = 0;
    for (auto& v : l) { CHECK(v == 99); ++cnt; }
    CHECK(cnt == 2);
}

void test_forward_list_assign_ilist() {
    std::forward_list<int> l;
    l.assign({1, 2, 3});
    CHECK(l.front() == 1);
}

void test_forward_list_before_begin() {
    std::forward_list<int> l = {1, 2, 3};
    auto bb = l.before_begin();
    auto b  = l.begin();
    CHECK(++bb == b);
}

void test_forward_list_front() {
    std::forward_list<int> l = {42, 1, 2};
    CHECK(l.front() == 42);
    l.front() = 0;
    CHECK(l.front() == 0);
}

void test_forward_list_push_pop_front() {
    std::forward_list<int> l;
    l.push_front(2);
    l.push_front(1);
    CHECK(l.front() == 1);
    l.pop_front();
    CHECK(l.front() == 2);
    l.pop_front();
    CHECK(l.empty());
}

void test_forward_list_emplace_front() {
    std::forward_list<std::pair<int,int>> l;
    auto& r = l.emplace_front(1, 2);
    CHECK(r.first == 1);
    CHECK(r.second == 2);
    CHECK(l.front().first == 1);
}

void test_forward_list_insert_after_single() {
    std::forward_list<int> l = {1, 3};
    auto it = l.insert_after(l.begin(), 2);
    CHECK(*it == 2);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
    CHECK(i == 4);
}

void test_forward_list_insert_after_n() {
    std::forward_list<int> l = {0, 4};
    l.insert_after(l.begin(), 2, 99);
    auto it = l.begin();
    CHECK(*it++ == 0);
    CHECK(*it++ == 99);
    CHECK(*it++ == 99);
    CHECK(*it++ == 4);
    CHECK(it == l.end());
}

void test_forward_list_insert_after_range() {
    std::forward_list<int> l = {1, 5};
    int arr[] = {2, 3, 4};
    l.insert_after(l.begin(), arr, arr + 3);
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
    CHECK(i == 6);
}

void test_forward_list_insert_after_ilist() {
    std::forward_list<int> l = {1, 5};
    l.insert_after(l.begin(), {2, 3, 4});
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
    CHECK(i == 6);
}

void test_forward_list_emplace_after() {
    std::forward_list<std::pair<int,int>> l = {{1,1}};
    auto it = l.emplace_after(l.begin(), 2, 3);
    CHECK(it->first == 2);
    CHECK(it->second == 3);
}

void test_forward_list_erase_after_single() {
    std::forward_list<int> l = {1, 2, 3};
    auto r = l.erase_after(l.begin());
    CHECK(*r == 3);
    CHECK(l.front() == 1);
    int cnt = 0;
    for (auto it = l.begin(); it != l.end(); ++it) ++cnt;
    CHECK(cnt == 2);
}

void test_forward_list_erase_after_range() {
    std::forward_list<int> l = {1, 2, 3, 4, 5};
    // erase (1, 5) exclusive = {2,3,4}, keep 1 and 5
    auto f = l.begin(); // points to 1
    auto la = l.begin();
    ++la; ++la; ++la; ++la; // points to 5
    l.erase_after(f, la);
    int cnt = 0;
    for (auto it = l.begin(); it != l.end(); ++it) ++cnt;
    CHECK(cnt == 2);
    CHECK(l.front() == 1);
}

void test_forward_list_resize() {
    std::forward_list<int> l = {1, 2, 3};
    l.resize(5, 0);
    int cnt = 0;
    for (auto it = l.begin(); it != l.end(); ++it) ++cnt;
    CHECK(cnt == 5);
    l.resize(2);
    cnt = 0;
    for (auto it = l.begin(); it != l.end(); ++it) ++cnt;
    CHECK(cnt == 2);
    CHECK(l.front() == 1);
}

void test_forward_list_clear() {
    std::forward_list<int> l = {1, 2, 3};
    l.clear();
    CHECK(l.empty());
}

void test_forward_list_swap() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b = {4, 5};
    a.swap(b);
    CHECK(a.front() == 4);
    CHECK(b.front() == 1);
}

void test_forward_list_nonmember_swap() {
    std::forward_list<int> a = {1, 2};
    std::forward_list<int> b = {3, 4, 5};
    std::swap(a, b);
    CHECK(a.front() == 3);
    CHECK(b.front() == 1);
}

void test_forward_list_splice_after_whole() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b = {4, 5, 6};
    a.splice_after(std::next(a.before_begin(), 3), b);
    CHECK(b.empty());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
    CHECK(i == 7);
}

void test_forward_list_splice_after_single() {
    std::forward_list<int> a = {1, 3};
    std::forward_list<int> b = {2};
    a.splice_after(a.begin(), b, b.before_begin());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
    CHECK(i == 4);
}

void test_forward_list_splice_after_range() {
    std::forward_list<int> a = {1, 5};
    std::forward_list<int> b = {2, 3, 4};
    a.splice_after(a.begin(), b, b.before_begin(), b.end());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
    CHECK(i == 6);
    CHECK(b.empty());
}

void test_forward_list_remove() {
    std::forward_list<int> l = {1, 2, 3, 2, 4, 2};
    auto n = l.remove(2);
    CHECK(n == 3);
    for (auto& v : l) CHECK(v != 2);
}

void test_forward_list_remove_if() {
    std::forward_list<int> l = {1, 2, 3, 4, 5};
    auto n = l.remove_if([](int x){ return x % 2 == 0; });
    CHECK(n == 2);
    for (auto& v : l) CHECK(v % 2 == 1);
}

void test_forward_list_unique() {
    std::forward_list<int> l = {1, 1, 2, 3, 3, 3, 4};
    auto n = l.unique();
    CHECK(n == 3);
    int vals[] = {1, 2, 3, 4};
    int i = 0;
    for (auto& v : l) CHECK(v == vals[i++]);
    CHECK(i == 4);
}

void test_forward_list_merge() {
    std::forward_list<int> a = {1, 3, 5};
    std::forward_list<int> b = {2, 4, 6};
    a.merge(b);
    CHECK(b.empty());
    int i = 1;
    for (auto& v : a) CHECK(v == i++);
    CHECK(i == 7);
}

void test_forward_list_sort() {
    std::forward_list<int> l = {5, 3, 1, 4, 2};
    l.sort();
    int i = 1;
    for (auto& v : l) CHECK(v == i++);
    CHECK(i == 6);
}

void test_forward_list_sort_comp() {
    std::forward_list<int> l = {1, 2, 3, 4, 5};
    l.sort(std::greater<int>{});
    int i = 5;
    for (auto& v : l) CHECK(v == i--);
    CHECK(i == 0);
}

void test_forward_list_reverse() {
    std::forward_list<int> l = {1, 2, 3, 4, 5};
    l.reverse();
    int i = 5;
    for (auto& v : l) CHECK(v == i--);
    CHECK(i == 0);
}

void test_forward_list_comparisons() {
    std::forward_list<int> a = {1, 2, 3};
    std::forward_list<int> b = {1, 2, 3};
    std::forward_list<int> c = {1, 2, 4};
    CHECK(a == b);
    CHECK(!(a == c));
    auto r = (a <=> c);
    CHECK(r < 0);
    CHECK((c <=> a) > 0);
    CHECK((a <=> b) == 0);
}

void test_forward_list_erase_free() {
    std::forward_list<int> l = {1, 2, 3, 2, 4};
    auto n = std::erase(l, 2);
    CHECK(n == 2);
    for (auto& v : l) CHECK(v != 2);
}

void test_forward_list_erase_if_free() {
    std::forward_list<int> l = {1, 2, 3, 4, 5};
    auto n = std::erase_if(l, [](int x){ return x > 3; });
    CHECK(n == 2);
    for (auto& v : l) CHECK(v <= 3);
}

void test_forward_list_deduction_guide() {
    int arr[] = {1, 2, 3};
    std::forward_list l(arr, arr + 3);
    CHECK(l.front() == 1);
}

void test_forward_list_move_only() {
    struct MoveOnly {
        int val;
        MoveOnly(int v) : val(v) {}
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;
    };
    std::forward_list<MoveOnly> l;
    l.emplace_front(3);
    l.emplace_front(2);
    l.emplace_front(1);
    CHECK(l.front().val == 1);
}
