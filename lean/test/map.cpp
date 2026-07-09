// lean/test/map.cpp — map / multimap for the lean profile.
// White-box: the rb-invariant checker reaches the embedded header node through
// the container address (the container's sole member is the tree, whose first
// member is the header node), then walks it with the std::detail node helpers.
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <cstdint>
#include "lean_test.h"

using namespace std;

// ------------------------------------------------------------------
// Red-black invariant checker (walks the actual node structure).
// ------------------------------------------------------------------
namespace {
using NB = std::detail::tree_node_base;

int rb_black_height(NB* n, NB* parent) {
    if (!n) return 1;                                   // null leaf: 1 black
    CHECK(std::detail::tree_parent(n) == parent);       // parent links consistent
    bool red = std::detail::tree_is_red(n);
    if (red) {                                          // no red-red edge
        CHECK(!n->left  || !std::detail::tree_is_red(n->left));
        CHECK(!n->right || !std::detail::tree_is_red(n->right));
    }
    int lh = rb_black_height(n->left, n);
    int rh = rb_black_height(n->right, n);
    CHECK(lh == rh);                                    // equal black height
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
    CHECK(!std::detail::tree_is_red(root));             // root is black
    rb_black_height(root, header);
    CHECK(std::detail::tree_min(root) == header->left); // cached leftmost correct
    CHECK(std::detail::tree_max(root) == header->right);// cached rightmost correct
}

// xorshift64 — deterministic, no <random> dependency.
uint64_t rng_state = 0x9e3779b97f4a7c15ull;
uint32_t rnd() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 7;
    rng_state ^= rng_state << 17;
    return static_cast<uint32_t>(rng_state >> 32);
}
} // namespace

// ------------------------------------------------------------------
// Instrumented value types.
// ------------------------------------------------------------------
namespace {
struct MoveOnly {
    int v = -1;
    MoveOnly() = default;
    explicit MoveOnly(int x) : v(x) {}
    MoveOnly(MoveOnly&& o) noexcept : v(o.v) { o.v = -1; }
    MoveOnly& operator=(MoveOnly&& o) noexcept { v = o.v; o.v = -1; return *this; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
};

struct Counted {
    static inline int live = 0;
    int v;
    Counted() : v(0) { ++live; }
    explicit Counted(int x) : v(x) { ++live; }
    Counted(const Counted& o) : v(o.v) { ++live; }
    Counted(Counted&& o) noexcept : v(o.v) { ++live; }
    Counted& operator=(const Counted&) = default;
    Counted& operator=(Counted&&) noexcept = default;
    ~Counted() { --live; }
};
} // namespace

// ------------------------------------------------------------------
// Basic operations, ordering, lookup, erase.
// ------------------------------------------------------------------
void test_map_basic() {
    map<int, int> m;
    CHECK(m.empty());
    for (int k : {5, 1, 9, 3, 7, 2, 8, 4, 6, 0})
        m[k] = k * 10;
    CHECK(m.size() == 10);
    check_rb(m);

    int expect = 0;
    for (auto& kv : m) { CHECK(kv.first == expect); CHECK(kv.second == expect * 10); ++expect; }
    CHECK(expect == 10);

    // reverse iteration + --end()
    auto it = m.end();
    --it;
    CHECK(it->first == 9);
    int rexp = 9;
    for (auto r = m.rbegin(); r != m.rend(); ++r) { CHECK(r->first == rexp); --rexp; }
    CHECK(rexp == -1);

    CHECK(m.find(4)->second == 40);
    CHECK(m.find(100) == m.end());
    CHECK(m.count(4) == 1);
    CHECK(m.count(100) == 0);
    CHECK(m.contains(7));
    CHECK(!m.contains(42));
    CHECK(m.at(6) == 60);

    CHECK(m.lower_bound(4)->first == 4);
    CHECK(m.upper_bound(4)->first == 5);
    auto er = m.equal_range(4);
    CHECK(er.first->first == 4 && er.second->first == 5);

    // erase by key / iterator / range
    CHECK(m.erase(0) == 1);
    CHECK(m.erase(0) == 0);
    check_rb(m);
    auto nx = m.erase(m.find(5));
    CHECK(nx->first == 6);
    check_rb(m);
    // erase range [2,4)
    m.erase(m.find(2), m.find(4));
    CHECK(!m.contains(2) && !m.contains(3) && m.contains(4));
    check_rb(m);

    m.clear();
    CHECK(m.empty());
    check_rb(m);
}

void test_map_insert_variants() {
    map<int, int> m;
    auto [i1, ok1] = m.insert({1, 10});
    CHECK(ok1 && i1->second == 10);
    auto [i2, ok2] = m.insert({1, 99});   // duplicate key -> no change
    CHECK(!ok2 && i2->second == 10);

    // insert with hint
    auto h = m.insert(m.end(), {2, 20});
    CHECK(h->second == 20);

    // try_emplace: no overwrite, no construction of mapped on hit
    auto [t1, tok1] = m.try_emplace(3, 30);
    CHECK(tok1 && t1->second == 30);
    auto [t2, tok2] = m.try_emplace(3, 999);
    CHECK(!tok2 && t2->second == 30);

    // insert_or_assign: overwrites
    auto [a1, aok1] = m.insert_or_assign(3, 33);
    CHECK(!aok1 && a1->second == 33);
    auto [a2, aok2] = m.insert_or_assign(4, 44);
    CHECK(aok2 && a2->second == 44);

    // emplace
    auto [e1, eok1] = m.emplace(5, 50);
    CHECK(eok1 && e1->second == 50);
    auto [e2, eok2] = m.emplace(5, 51);
    CHECK(!eok2 && e2->second == 50);
    check_rb(m);
}

void test_map_emplace_hint() {
    map<int, int> m;
    // Fill ascending with a correct end() hint (amortized O(1) path).
    for (int i = 0; i < 200; ++i) {
        auto it = m.emplace_hint(m.end(), i, i);
        CHECK(it->first == i);
    }
    CHECK(m.size() == 200);
    check_rb(m);
    // wrong hints must still produce correct results
    auto it = m.emplace_hint(m.begin(), 250, 250);
    CHECK(it->first == 250);
    it = m.try_emplace(m.begin(), 251, 251);
    CHECK(it->first == 251);
    // hint duplicate
    it = m.emplace_hint(m.find(100), 100, 999);
    CHECK(it->first == 100 && it->second == 100);
    check_rb(m);
    int prev = -1;
    for (auto& kv : m) { CHECK(kv.first > prev); prev = kv.first; }
}

// ------------------------------------------------------------------
// Copy / move / swap / assignment / comparisons.
// ------------------------------------------------------------------
void test_map_copy_move_swap() {
    map<int, int> a;
    for (int i = 0; i < 50; ++i) a[i] = i;

    map<int, int> b = a;                  // copy ctor
    CHECK(b == a);
    check_rb(b);
    b[999] = 1;
    CHECK(!(b == a));

    map<int, int> c = move(a);            // move ctor
    CHECK(c.size() == 50);
    CHECK(a.empty());                     // moved-from is empty & usable
    check_rb(a);
    check_rb(c);
    a[7] = 7;
    CHECK(a.size() == 1);

    map<int, int> d;
    d = c;                                // copy assign
    CHECK(d == c);
    d = move(c);                          // move assign
    CHECK(d.size() == 50);
    check_rb(d);

    // swap
    map<int, int> x, y;
    x[1] = 1; x[2] = 2;
    y[10] = 10;
    swap(x, y);
    CHECK(x.size() == 1 && x.contains(10));
    CHECK(y.size() == 2 && y.contains(1) && y.contains(2));
    check_rb(x); check_rb(y);
    // swap with empty
    map<int, int> empty1;
    swap(x, empty1);
    CHECK(x.empty() && empty1.size() == 1);
    check_rb(x); check_rb(empty1);

    // comparisons
    map<int, int> p{{1, 1}, {2, 2}}, q{{1, 1}, {2, 3}};
    CHECK(p < q);
    CHECK((p <=> q) < 0);
    CHECK(p != q);
}

void test_map_erase_if() {
    map<int, int> m;
    for (int i = 0; i < 100; ++i) m[i] = i;
    auto removed = erase_if(m, [](const pair<const int, int>& kv) { return kv.first % 2 == 0; });
    CHECK(removed == 50);
    CHECK(m.size() == 50);
    for (auto& kv : m) CHECK(kv.first % 2 == 1);
    check_rb(m);
}

// ------------------------------------------------------------------
// Non-trivial mapped type + reference/iterator stability.
// ------------------------------------------------------------------
void test_map_nontrivial_and_stability() {
    {
        map<int, string> m;
        for (int i = 0; i < 40; ++i) m[i] = "value-" + to_string(i);
        // pointers/iterators to existing values must survive other mutations
        string* p10 = &m[10];
        string* p20 = &m[20];
        auto it30 = m.find(30);
        for (int i = 1000; i < 1200; ++i) m[i] = "x";   // many inserts
        for (int i = 0; i < 40; i += 3) if (i != 10 && i != 20 && i != 30) m.erase(i);
        CHECK(*p10 == "value-10");                       // address/value stable
        CHECK(*p20 == "value-20");
        CHECK(it30->second == "value-30");
        CHECK(&m[10] == p10);                            // same node, same address
        check_rb(m);
    }
    CHECK(true);

    // Counted: verify no leaks across construction/destruction/erase.
    Counted::live = 0;
    {
        map<int, Counted> m;
        for (int i = 0; i < 100; ++i) m.try_emplace(i, i);
        CHECK(Counted::live == 100);
        for (int i = 0; i < 50; ++i) m.erase(i);
        CHECK(Counted::live == 50);
        map<int, Counted> m2 = m;                        // copy
        CHECK(Counted::live == 100);
    }
    CHECK(Counted::live == 0);
}

void test_map_move_only_value() {
    map<int, MoveOnly> m;
    m.try_emplace(1, 100);
    m.emplace(2, MoveOnly(200));
    m[3] = MoveOnly(300);                                 // operator[] + move assign
    CHECK(m.at(1).v == 100);
    CHECK(m.at(2).v == 200);
    CHECK(m.at(3).v == 300);
    CHECK(m.size() == 3);
    // move the whole container
    map<int, MoveOnly> m2 = move(m);
    CHECK(m2.at(2).v == 200);
    CHECK(m.empty());
    check_rb(m2);
}

// ------------------------------------------------------------------
// Heterogeneous lookup: map<string, int, less<>>.
// ------------------------------------------------------------------
void test_map_string_heterogeneous() {
    map<string, int, less<>> m;
    m["apple"] = 1;
    m["banana"] = 2;
    m["cherry"] = 3;
    m["date"] = 4;

    // look up with types other than std::string (no std::string constructed)
    const char* c = "banana";
    CHECK(m.find(c)->second == 2);
    CHECK(m.find(string_view("cherry"))->second == 3);
    CHECK(m.count(string_view("apple")) == 1);
    CHECK(m.count(string_view("zzz")) == 0);
    CHECK(m.contains(string_view("date")));

    auto lo = m.lower_bound(string_view("b"));
    CHECK(lo->first == "banana");
    auto up = m.upper_bound(string_view("cherry"));
    CHECK(up->first == "date");
    auto er = m.equal_range(string_view("cherry"));
    CHECK(er.first->first == "cherry" && er.second->first == "date");

    // ordering is lexicographic
    string prev;
    bool first = true;
    for (auto& kv : m) { if (!first) CHECK(prev < kv.first); prev = kv.first; first = false; }
    check_rb(m);
}

// ------------------------------------------------------------------
// multimap: equal-range semantics, count, erase(key) removes all.
// ------------------------------------------------------------------
void test_multimap() {
    multimap<int, int> mm;
    // insert duplicates; equal elements must stay in insertion order.
    for (int i = 0; i < 5; ++i) mm.emplace(1, i);
    mm.emplace(2, 100);
    for (int i = 0; i < 3; ++i) mm.emplace(3, i);
    CHECK(mm.size() == 9);
    check_rb(mm);

    CHECK(mm.count(1) == 5);
    CHECK(mm.count(2) == 1);
    CHECK(mm.count(3) == 3);
    CHECK(mm.count(4) == 0);

    auto er = mm.equal_range(1);
    int seen = 0;
    for (auto it = er.first; it != er.second; ++it) { CHECK(it->first == 1); CHECK(it->second == seen); ++seen; }
    CHECK(seen == 5);                                     // stable insertion order preserved

    // erase(key) removes the whole equal range
    CHECK(mm.erase(1) == 5);
    CHECK(mm.count(1) == 0);
    CHECK(mm.size() == 4);
    check_rb(mm);

    // hint emplace into the middle keeps order
    mm.emplace_hint(mm.end(), 2, 101);
    CHECK(mm.count(2) == 2);
    check_rb(mm);
}

// ------------------------------------------------------------------
// 10k+ randomized stress vs a manually maintained reference array.
// Every operation is mirrored; full equality + rb-invariant are checked
// along the way.
// ------------------------------------------------------------------
void test_map_stress() {
    static const int DOMAIN = 25000;
    static bool present[DOMAIN];
    static int  refval[DOMAIN];
    for (int i = 0; i < DOMAIN; ++i) present[i] = false;

    map<int, int> m;
    size_t live = 0;

    auto full_check = [&]() {
        CHECK(m.size() == live);
        // iterate map, compare to reference in ascending key order
        size_t idx = 0;
        auto it = m.begin();
        for (int k = 0; k < DOMAIN; ++k) {
            if (!present[k]) continue;
            CHECK(it != m.end());
            CHECK(it->first == k);
            CHECK(it->second == refval[k]);
            ++it; ++idx;
        }
        CHECK(it == m.end());
        CHECK(idx == live);
        check_rb(m);
    };

    for (int op = 0; op < 60000; ++op) {
        int k = rnd() % DOMAIN;
        bool doInsert = (rnd() % 100) < 55;
        if (doInsert) {
            int v = static_cast<int>(rnd());
            auto [it, ok] = m.insert({k, v});
            if (present[k]) {
                CHECK(!ok);
                CHECK(it->second == refval[k]);            // insert must not overwrite
            } else {
                CHECK(ok);
                present[k] = true; refval[k] = v; ++live;
            }
        } else {
            size_t erased = m.erase(k);
            if (present[k]) { CHECK(erased == 1); present[k] = false; --live; }
            else            { CHECK(erased == 0); }
        }
        if (op % 300 == 0) full_check();
    }
    // ensure the stress actually built a large tree at some point
    CHECK(live > 10000);
    full_check();

    // drain everything via erase(iterator) and confirm it empties cleanly
    while (!m.empty()) {
        auto it = m.begin();
        int k = it->first;
        CHECK(present[k]);
        m.erase(it);
        present[k] = false; --live;
        if ((rnd() & 0x3ff) == 0) check_rb(m);
    }
    CHECK(live == 0);
    check_rb(m);
}

// ------------------------------------------------------------------
// Hint fast-path at scale: insert many random keys each with the correct
// lower_bound() hint, mirrored against a bool array; order + invariant checked.
// ------------------------------------------------------------------
void test_map_hint_stress() {
    static const int DOMAIN = 12000;
    static bool present[DOMAIN];
    for (int i = 0; i < DOMAIN; ++i) present[i] = false;

    map<int, int> m;
    size_t live = 0;
    for (int op = 0; op < 20000; ++op) {
        int k = rnd() % DOMAIN;
        auto hint = m.lower_bound(k);                 // the correct hint
        auto it = m.emplace_hint(hint, k, k * 2);
        CHECK(it->first == k);
        CHECK(it->second == k * 2);      // value is k*2 whether inserted or a hit
        if (!present[k]) { present[k] = true; ++live; }
        if (op % 400 == 0) {
            CHECK(m.size() == live);
            int prev = -1;
            for (auto& kv : m) { CHECK(kv.first > prev); prev = kv.first; }
            check_rb(m);
        }
    }
    CHECK(m.size() == live);
    // multimap hint fast-path: duplicates appended in order
    multimap<int, int> mm;
    for (int i = 0; i < 4000; ++i) {
        int k = rnd() % 500;
        mm.emplace_hint(mm.upper_bound(k), k, i);
    }
    int prev = -1;
    for (auto& kv : mm) { CHECK(kv.first >= prev); prev = kv.first; }
    check_rb(mm);
}

// ------------------------------------------------------------------
// sizeof contract.
// ------------------------------------------------------------------
void test_map_sizeof() {
    static_assert(sizeof(map<int, int>) <= 40);
    static_assert(sizeof(multimap<int, int>) <= 40);
    CHECK(sizeof(map<int, int>) <= 40);
}

int main() {
    test_map_basic();
    test_map_insert_variants();
    test_map_emplace_hint();
    test_map_copy_move_swap();
    test_map_erase_if();
    test_map_nontrivial_and_stability();
    test_map_move_only_value();
    test_map_string_heterogeneous();
    test_multimap();
    test_map_stress();
    test_map_hint_stress();
    test_map_sizeof();
    return 0;
}
