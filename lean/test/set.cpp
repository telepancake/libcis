// lean/test/set.cpp — set / multiset for the lean profile.
// White-box rb-invariant checker as in map.cpp (reaches the embedded header
// node through the container address).
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <cstdint>
#include "lean_test.h"

using namespace std;

namespace {
using NB = std::detail::tree_node_base;

int rb_black_height(NB* n, NB* parent) {
    if (!n) return 1;
    CHECK(std::detail::tree_parent(n) == parent);
    bool red = std::detail::tree_is_red(n);
    if (red) {
        CHECK(!n->left  || !std::detail::tree_is_red(n->left));
        CHECK(!n->right || !std::detail::tree_is_red(n->right));
    }
    int lh = rb_black_height(n->left, n);
    int rh = rb_black_height(n->right, n);
    CHECK(lh == rh);
    return lh + (red ? 0 : 1);
}

template<class C>
void check_rb(const C& c) {
    NB* header = reinterpret_cast<NB*>(const_cast<C*>(&c));
    NB* root = std::detail::tree_parent(header);
    if (!root) {
        CHECK(c.size() == 0);
        CHECK(header->left == header && header->right == header);
        return;
    }
    CHECK(!std::detail::tree_is_red(root));
    rb_black_height(root, header);
    CHECK(std::detail::tree_min(root) == header->left);
    CHECK(std::detail::tree_max(root) == header->right);
}

uint64_t rng_state = 0xd1b54a32d192ed03ull;
uint32_t rnd() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return static_cast<uint32_t>(rng_state >> 32);
}

// Move-only, comparable key type.
struct MoveOnly {
    int v = -1;
    MoveOnly() = default;
    explicit MoveOnly(int x) : v(x) {}
    MoveOnly(MoveOnly&& o) noexcept : v(o.v) { o.v = -1; }
    MoveOnly& operator=(MoveOnly&& o) noexcept { v = o.v; o.v = -1; return *this; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    friend bool operator<(const MoveOnly& a, const MoveOnly& b) { return a.v < b.v; }
};

struct Counted {
    static inline int live = 0;
    int v;
    explicit Counted(int x) : v(x) { ++live; }
    Counted(const Counted& o) : v(o.v) { ++live; }
    Counted(Counted&& o) noexcept : v(o.v) { ++live; }
    ~Counted() { --live; }
    friend bool operator<(const Counted& a, const Counted& b) { return a.v < b.v; }
};
} // namespace

// ------------------------------------------------------------------
void test_set_basic() {
    set<int> s;
    CHECK(s.empty());
    for (int k : {5, 1, 9, 3, 7, 2, 8, 4, 6, 0}) s.insert(k);
    CHECK(s.size() == 10);
    check_rb(s);

    int expect = 0;
    for (int v : s) { CHECK(v == expect); ++expect; }
    CHECK(expect == 10);

    // duplicate insert is a no-op
    auto [it, ok] = s.insert(5);
    CHECK(!ok && *it == 5);
    CHECK(s.size() == 10);

    // --end() and reverse iteration
    auto e = s.end(); --e;
    CHECK(*e == 9);
    int rexp = 9;
    for (auto r = s.rbegin(); r != s.rend(); ++r) { CHECK(*r == rexp); --rexp; }

    CHECK(s.find(4) != s.end());
    CHECK(s.find(42) == s.end());
    CHECK(s.count(7) == 1);
    CHECK(s.contains(3));
    CHECK(!s.contains(99));
    CHECK(*s.lower_bound(4) == 4);
    CHECK(*s.upper_bound(4) == 5);
    auto er = s.equal_range(4);
    CHECK(*er.first == 4 && *er.second == 5);

    CHECK(s.erase(0) == 1);
    CHECK(s.erase(0) == 0);
    auto nx = s.erase(s.find(5));
    CHECK(*nx == 6);
    s.erase(s.find(2), s.find(4));
    CHECK(!s.contains(2) && !s.contains(3) && s.contains(4));
    check_rb(s);

    s.clear();
    CHECK(s.empty());
    check_rb(s);
}

void test_set_emplace_hint() {
    set<int> s;
    for (int i = 0; i < 300; ++i) {
        auto it = s.emplace_hint(s.end(), i);
        CHECK(*it == i);
    }
    CHECK(s.size() == 300);
    check_rb(s);
    auto [it, ok] = s.emplace(150);
    CHECK(!ok && *it == 150);
    it = s.emplace_hint(s.begin(), -1);
    CHECK(*it == -1);
    check_rb(s);
    int prev = -2;                       // hint inserts must preserve sorted order
    for (int v : s) { CHECK(v > prev); prev = v; }
}

void test_set_copy_move_swap() {
    set<int> a;
    for (int i = 0; i < 60; ++i) a.insert(i);

    set<int> b = a;
    CHECK(b == a);
    check_rb(b);
    b.insert(1000);
    CHECK(!(b == a));
    CHECK(a < b);
    CHECK((a <=> b) < 0);

    set<int> c = move(a);
    CHECK(c.size() == 60);
    CHECK(a.empty());
    check_rb(a); check_rb(c);

    set<int> d;
    d = c;
    CHECK(d == c);
    d = move(c);
    CHECK(d.size() == 60);
    check_rb(d);

    set<int> x{1, 2, 3}, y{9};
    swap(x, y);
    CHECK(x.size() == 1 && x.contains(9));
    CHECK(y.size() == 3);
    check_rb(x); check_rb(y);
    set<int> empt;
    swap(x, empt);
    CHECK(x.empty() && empt.size() == 1);
    check_rb(x); check_rb(empt);
}

void test_set_erase_if_and_counted() {
    set<int> s;
    for (int i = 0; i < 100; ++i) s.insert(i);
    auto removed = erase_if(s, [](int v) { return v % 3 == 0; });
    CHECK(removed == 34);
    for (int v : s) CHECK(v % 3 != 0);
    check_rb(s);

    Counted::live = 0;
    {
        set<Counted> cs;
        for (int i = 0; i < 80; ++i) cs.insert(Counted(i));
        CHECK(Counted::live == 80);
        set<Counted> cs2 = cs;
        CHECK(Counted::live == 160);
        for (int i = 0; i < 40; ++i) cs.erase(Counted(i));
        CHECK(Counted::live == 120);
    }
    CHECK(Counted::live == 0);
}

void test_set_nontrivial_and_stability() {
    set<string> s;
    for (int i = 0; i < 50; ++i) s.insert("k" + to_string(1000 + i));
    const string* p = &*s.find("k1010");
    for (int i = 0; i < 50; ++i) s.insert("z" + to_string(i));    // more inserts
    s.erase("k1005");
    s.erase("k1040");
    CHECK(*p == "k1010");                                          // reference stable
    CHECK(&*s.find("k1010") == p);
    check_rb(s);

    string prev; bool first = true;
    for (auto& v : s) { if (!first) CHECK(prev < v); prev = v; first = false; }
}

void test_set_move_only() {
    set<MoveOnly> s;
    s.insert(MoveOnly(3));
    s.insert(MoveOnly(1));
    s.emplace(2);
    s.insert(MoveOnly(1));                        // duplicate
    CHECK(s.size() == 3);
    CHECK(s.contains(MoveOnly(2)));
    CHECK(s.find(MoveOnly(5)) == s.end());
    // ordered
    int expect = 1;
    for (auto& mo : s) { CHECK(mo.v == expect); ++expect; }
    CHECK(s.erase(MoveOnly(1)) == 1);
    CHECK(s.size() == 2);

    set<MoveOnly> s2 = move(s);
    CHECK(s2.size() == 2);
    CHECK(s.empty());
    check_rb(s2);
}

// ------------------------------------------------------------------
// multiset: equal-range semantics, count, erase(key) removes all,
// erase(iterator) removes one.
// ------------------------------------------------------------------
void test_multiset() {
    multiset<int> ms;
    for (int i = 0; i < 4; ++i) ms.insert(1);
    ms.insert(2);
    for (int i = 0; i < 3; ++i) ms.insert(3);
    CHECK(ms.size() == 8);
    check_rb(ms);

    CHECK(ms.count(1) == 4);
    CHECK(ms.count(3) == 3);
    CHECK(ms.count(9) == 0);

    auto er = ms.equal_range(1);
    int seen = 0;
    for (auto it = er.first; it != er.second; ++it) { CHECK(*it == 1); ++seen; }
    CHECK(seen == 4);
    CHECK(er.second == ms.lower_bound(2));

    // erase(iterator) removes exactly one
    ms.erase(ms.find(1));
    CHECK(ms.count(1) == 3);
    CHECK(ms.size() == 7);
    check_rb(ms);

    // erase(key) removes all equal
    CHECK(ms.erase(3) == 3);
    CHECK(ms.count(3) == 0);
    check_rb(ms);

    // ordered iteration with multiplicity
    multiset<int> m2{4, 1, 4, 2, 1, 1, 3};
    int prev = -1;
    for (int v : m2) { CHECK(v >= prev); prev = v; }
    CHECK(m2.count(1) == 3 && m2.count(4) == 2);
}

// ------------------------------------------------------------------
// 10k+ randomized stress: set vs a bool-array reference model.
// ------------------------------------------------------------------
void test_set_stress() {
    static const int DOMAIN = 25000;
    static bool present[DOMAIN];
    for (int i = 0; i < DOMAIN; ++i) present[i] = false;

    set<int> s;
    size_t live = 0;

    auto full_check = [&]() {
        CHECK(s.size() == live);
        auto it = s.begin();
        size_t idx = 0;
        for (int k = 0; k < DOMAIN; ++k) {
            if (!present[k]) continue;
            CHECK(it != s.end());
            CHECK(*it == k);
            ++it; ++idx;
        }
        CHECK(it == s.end());
        CHECK(idx == live);
        check_rb(s);
    };

    for (int op = 0; op < 60000; ++op) {
        int k = rnd() % DOMAIN;
        if ((rnd() % 100) < 55) {
            auto [it, ok] = s.insert(k);
            CHECK(*it == k);
            if (present[k]) CHECK(!ok);
            else { CHECK(ok); present[k] = true; ++live; }
        } else {
            size_t e = s.erase(k);
            if (present[k]) { CHECK(e == 1); present[k] = false; --live; }
            else            { CHECK(e == 0); }
        }
        if (op % 300 == 0) full_check();
    }
    CHECK(live > 10000);
    full_check();

    while (!s.empty()) {
        auto it = s.begin();
        int k = *it;
        s.erase(it);
        present[k] = false; --live;
        if ((rnd() & 0x3ff) == 0) check_rb(s);
    }
    CHECK(live == 0);
    check_rb(s);
}

// ------------------------------------------------------------------
// multiset stress with duplicates: mirror per-key multiplicities.
// ------------------------------------------------------------------
void test_multiset_stress() {
    static const int DOMAIN = 1500;
    static int cnt[DOMAIN];
    for (int i = 0; i < DOMAIN; ++i) cnt[i] = 0;

    multiset<int> ms;
    size_t total = 0;
    size_t peak = 0;

    auto full_check = [&]() {
        CHECK(ms.size() == total);
        auto it = ms.begin();
        for (int k = 0; k < DOMAIN; ++k) {
            for (int c = 0; c < cnt[k]; ++c) {
                CHECK(it != ms.end());
                CHECK(*it == k);
                ++it;
            }
            CHECK(ms.count(k) == static_cast<size_t>(cnt[k]));
        }
        CHECK(it == ms.end());
        check_rb(ms);
    };

    for (int op = 0; op < 50000; ++op) {
        int k = rnd() % DOMAIN;
        int roll = rnd() % 100;
        if (roll < 72) {                       // insert one
            ms.insert(k);
            ++cnt[k]; ++total;
        } else if (roll < 95) {                // erase one (if any)
            auto it = ms.find(k);
            if (it != ms.end()) {
                CHECK(*it == k);
                ms.erase(it);
                --cnt[k]; --total;
            } else {
                CHECK(cnt[k] == 0);
            }
        } else {                               // erase all of key k
            size_t e = ms.erase(k);
            CHECK(e == static_cast<size_t>(cnt[k]));
            total -= cnt[k]; cnt[k] = 0;
        }
        if (total > peak) peak = total;
        if (op % 400 == 0) full_check();
    }
    CHECK(peak > 10000);               // the tree held >10k elements at its peak
    full_check();

    // verify equal_range covers exactly the multiplicity for a few keys
    for (int t = 0; t < 20; ++t) {
        int k = rnd() % DOMAIN;
        auto er = ms.equal_range(k);
        size_t n = 0;
        for (auto it = er.first; it != er.second; ++it) { CHECK(*it == k); ++n; }
        CHECK(n == static_cast<size_t>(cnt[k]));
    }
    ms.clear();
    check_rb(ms);
}

void test_set_sizeof() {
    static_assert(sizeof(set<int>) <= 40);
    static_assert(sizeof(multiset<int>) <= 40);
    CHECK(sizeof(set<int>) <= 40);
}

int main() {
    test_set_basic();
    test_set_emplace_hint();
    test_set_copy_move_swap();
    test_set_erase_if_and_counted();
    test_set_nontrivial_and_stability();
    test_set_move_only();
    test_multiset();
    test_set_stress();
    test_multiset_stress();
    test_set_sizeof();
    return 0;
}
