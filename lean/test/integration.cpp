// Integration test: the cross-component matrix. Every lean container/algorithm
// meets every other through the overlay in a single TU. Failures trap via CHECK.
#include "lean_test.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <deque>
#include <algorithm>
#include <queue>
#include <stack>
#include <utility>

// ---- sizeof matrix (lean/README.md representation table, x86-64) ------------
static_assert(sizeof(std::string) == 8, "string is one pointer");
static_assert(sizeof(std::vector<int>) == 8, "vector is one pointer");
static_assert(sizeof(std::vector<std::string>) == 8, "vector<string> is one pointer");
static_assert(sizeof(std::unordered_map<std::string, int>) == 8, "unordered_map is one pointer");
static_assert(sizeof(std::unordered_set<int>) == 8, "unordered_set is one pointer");
static_assert(sizeof(std::map<std::string, std::vector<int>>) <= 40, "map <= 40");
static_assert(sizeof(std::set<int>) <= 40, "set <= 40");
static_assert(sizeof(std::list<std::string>) <= 24, "list <= 24");
static_assert(sizeof(std::deque<std::string>) <= 24, "deque <= 24");

// ---------------------------------------------------------------------------
static void test_vector_string() {
    std::vector<std::string> v;
    // growth well past initial capacity, all with lean std::string elements
    for (int i = 0; i < 500; ++i)
        v.push_back(std::string("item-") + std::to_string(i));
    CHECK(v.size() == 500);
    CHECK(v[0] == "item-0");
    CHECK(v[499] == "item-499");
    // insert at front (forces shift of 500 non-trivially-relocatable... actually
    // lean string IS trivially relocatable, so realloc/memmove path)
    v.insert(v.begin(), std::string("HEAD"));
    CHECK(v[0] == "HEAD");
    CHECK(v[1] == "item-0");
    CHECK(v.size() == 501);
    // insert in the middle
    v.insert(v.begin() + 250, std::string("MID"));
    CHECK(v[250] == "MID");
    CHECK(v.size() == 502);
    // erase front
    v.erase(v.begin());
    CHECK(v[0] == "item-0");
    // erase a range
    v.erase(v.begin() + 10, v.begin() + 20);
    CHECK(v.size() == 491);
    // self-referential push_back
    v.push_back(v[0]);
    CHECK(v.back() == "item-0");
    // resize shrink then grow with a string fill
    v.resize(100);
    CHECK(v.size() == 100);
    v.resize(150, std::string("pad"));
    CHECK(v[149] == "pad");
}

static void test_map_string_vector() {
    std::map<std::string, std::vector<int>> m;
    for (int i = 0; i < 200; ++i) {
        std::string k = "key" + std::to_string(i % 40);
        m[k].push_back(i);
    }
    CHECK(m.size() == 40);
    // each key got 5 values (200 / 40)
    for (auto& kv : m)
        CHECK(kv.second.size() == 5);
    // ordering is by string key
    std::string prev;
    bool first = true;
    for (auto& kv : m) {
        if (!first) CHECK(prev < kv.first);
        prev = kv.first;
        first = false;
    }
    // mutate a mapped vector, sort it
    auto& vv = m["key3"];
    std::sort(vv.begin(), vv.end());
    for (size_t i = 1; i < vv.size(); ++i) CHECK(vv[i - 1] <= vv[i]);
    // heterogeneous-ish lookup via string
    CHECK(m.find(std::string("key0")) != m.end());
    CHECK(m.count("nope") == 0);
    m.erase("key0");
    CHECK(m.size() == 39);
}

static void test_unordered_rehash() {
    std::unordered_map<std::string, int> um;
    for (int i = 0; i < 1000; ++i)
        um[std::string("s") + std::to_string(i)] = i;
    CHECK(um.size() == 1000);
    // force rehashes
    um.rehash(1 << 16);
    CHECK(um.size() == 1000);
    CHECK(um.bucket_count() >= (1u << 16));
    for (int i = 0; i < 1000; ++i)
        CHECK(um[std::string("s") + std::to_string(i)] == i);
    um.reserve(4000);
    CHECK(um.size() == 1000);
    // values survive rehash
    CHECK(um["s500"] == 500);
    um.max_load_factor(0.25f);
    um.rehash(0);  // shrink to fit load factor
    CHECK(um.size() == 1000);
    CHECK(um["s999"] == 999);
}

static void test_sort_strings() {
    std::vector<std::string> v;
    const char* words[] = {"pear","apple","cherry","banana","fig","date","apple","kiwi"};
    for (auto w : words) v.push_back(w);
    std::sort(v.begin(), v.end());
    for (size_t i = 1; i < v.size(); ++i) CHECK(v[i - 1] <= v[i]);
    CHECK(v.front() == "apple");
    // stable_sort by length keeps equal-length relative order
    std::vector<std::string> v2 = {"bb","a","cc","d","eee","ff"};
    std::stable_sort(v2.begin(), v2.end(),
                     [](const std::string& a, const std::string& b){ return a.size() < b.size(); });
    // lengths non-decreasing
    for (size_t i = 1; i < v2.size(); ++i) CHECK(v2[i-1].size() <= v2[i].size());
    // among length-2, original order was bb, cc, ff
    std::vector<std::string> twos;
    for (auto& s : v2) if (s.size() == 2) twos.push_back(s);
    CHECK((twos == std::vector<std::string>{"bb","cc","ff"}));
}

static void test_sort_pairs() {
    std::vector<std::pair<int,int>> v;
    for (int i = 0; i < 300; ++i)
        v.push_back({ (i * 7919) % 100, i });   // key collisions on first
    std::stable_sort(v.begin(), v.end(),
                     [](const std::pair<int,int>& a, const std::pair<int,int>& b){ return a.first < b.first; });
    for (size_t i = 1; i < v.size(); ++i) {
        CHECK(v[i-1].first <= v[i].first);
        // stability: equal first => original .second order preserved (ascending)
        if (v[i-1].first == v[i].first) CHECK(v[i-1].second < v[i].second);
    }
    // full sort of pairs (lexicographic)
    std::sort(v.begin(), v.end());
    for (size_t i = 1; i < v.size(); ++i) CHECK(!(v[i] < v[i-1]));
}

static void test_list_sort_splice() {
    std::list<std::string> a;
    for (int i = 0; i < 50; ++i) a.push_back(std::string("a") + std::to_string((i * 13) % 50));
    a.sort();
    // sorted
    std::string prev; bool first = true;
    for (auto& s : a) { if (!first) CHECK(prev <= s); prev = s; first = false; }
    std::list<std::string> b;
    for (int i = 0; i < 10; ++i) b.push_back(std::string("b") + std::to_string(i));
    size_t total = a.size() + b.size();
    // splice all of b into a at begin
    a.splice(a.begin(), b);
    CHECK(b.empty());
    CHECK(a.size() == total);
    CHECK(a.front() == "b0");
    // merge two sorted lists
    std::list<std::string> c = {"m1","m3","m5"};
    std::list<std::string> d = {"m2","m4","m6"};
    c.merge(d);
    CHECK(d.empty());
    CHECK((c == std::list<std::string>{"m1","m2","m3","m4","m5","m6"}));
}

static void test_deque_string() {
    std::deque<std::string> dq;
    for (int i = 0; i < 300; ++i) {
        dq.push_back(std::string("back") + std::to_string(i));
        dq.push_front(std::string("front") + std::to_string(i));
    }
    CHECK(dq.size() == 600);
    CHECK(dq.front() == "front299");
    CHECK(dq.back() == "back299");
    CHECK(dq[300] == "back0");
    CHECK(dq[299] == "front0");
    dq.pop_front();
    dq.pop_back();
    CHECK(dq.size() == 598);
    CHECK(dq.front() == "front298");
    CHECK(dq.back() == "back298");
}

static void test_queue_stack() {
    std::queue<int> q;   // over lean deque<int>
    for (int i = 0; i < 100; ++i) q.push(i);
    CHECK(q.front() == 0);
    CHECK(q.back() == 99);
    CHECK(q.size() == 100);
    int sum = 0;
    while (!q.empty()) { sum += q.front(); q.pop(); }
    CHECK(sum == 99 * 100 / 2);

    std::stack<int> s;   // over lean deque<int>
    for (int i = 0; i < 100; ++i) s.push(i);
    CHECK(s.top() == 99);
    CHECK(s.size() == 100);
    int expect = 99;
    while (!s.empty()) { CHECK(s.top() == expect); s.pop(); --expect; }
    CHECK(expect == -1);
}

static void test_erase_if() {
    std::vector<int> v;
    for (int i = 0; i < 100; ++i) v.push_back(i);
    auto nv = std::erase_if(v, [](int x){ return x % 2 == 0; });
    CHECK(nv == 50);
    CHECK(v.size() == 50);
    for (int x : v) CHECK(x % 2 == 1);

    std::list<int> l;
    for (int i = 0; i < 100; ++i) l.push_back(i);
    auto nl = std::erase_if(l, [](int x){ return x < 30; });
    CHECK(nl == 30);
    CHECK(l.size() == 70);
    CHECK(l.front() == 30);

    std::map<int,int> m;
    for (int i = 0; i < 100; ++i) m[i] = i;
    auto nm = std::erase_if(m, [](const std::pair<const int,int>& kv){ return kv.first >= 90; });
    CHECK(nm == 10);
    CHECK(m.size() == 90);

    std::set<int> st;
    for (int i = 0; i < 100; ++i) st.insert(i);
    auto ns = std::erase_if(st, [](int x){ return x % 3 == 0; });
    CHECK(ns == 34);  // 0,3,...,99 => 34 values
    CHECK(st.size() == 66);

    std::unordered_map<int,int> um;
    for (int i = 0; i < 100; ++i) um[i] = i;
    auto num = std::erase_if(um, [](const std::pair<const int,int>& kv){ return kv.second > 49; });
    CHECK(num == 50);
    CHECK(um.size() == 50);

    std::deque<int> dq;
    for (int i = 0; i < 100; ++i) dq.push_back(i);
    auto nd = std::erase_if(dq, [](int x){ return x % 10 == 0; });
    CHECK(nd == 10);
    CHECK(dq.size() == 90);
}

// ---- 50k mixed-op stress vs reference models --------------------------------
static unsigned rng_state = 0x1234567u;
static unsigned rng() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static void test_stress_vector_string() {
    std::vector<std::string> v;
    std::deque<std::string> ref;  // reference model (independent lean deque)
    for (int i = 0; i < 50000; ++i) {
        unsigned op = rng() % 10;
        if (op < 6 || v.empty()) {
            std::string s = std::to_string(rng() % 100000);
            v.push_back(s);
            ref.push_back(s);
        } else if (op < 8) {
            v.pop_back();
            ref.pop_back();
        } else {
            size_t idx = rng() % v.size();
            std::string s = "X" + std::to_string(rng() % 1000);
            v.insert(v.begin() + idx, s);
            ref.insert(ref.begin() + idx, s);
        }
    }
    CHECK(v.size() == ref.size());
    for (size_t i = 0; i < v.size(); ++i) CHECK(v[i] == ref[i]);
}

static void test_stress_map() {
    std::map<int,int> m;
    // reference: sorted unique keys via set + parallel values
    int model[2000];
    bool present[2000];
    for (int i = 0; i < 2000; ++i) { model[i] = 0; present[i] = false; }
    for (int i = 0; i < 50000; ++i) {
        int k = rng() % 2000;
        unsigned op = rng() % 3;
        if (op == 0) {  // erase
            m.erase(k);
            present[k] = false;
        } else {        // insert/assign
            int val = (int)(rng() % 1000000);
            m[k] = val;
            model[k] = val;
            present[k] = true;
        }
    }
    size_t cnt = 0;
    for (int i = 0; i < 2000; ++i) if (present[i]) ++cnt;
    CHECK(m.size() == cnt);
    for (int i = 0; i < 2000; ++i) {
        auto it = m.find(i);
        if (present[i]) { CHECK(it != m.end()); CHECK(it->second == model[i]); }
        else CHECK(it == m.end());
    }
    // in-order and sorted
    int prev = -1;
    for (auto& kv : m) { CHECK(kv.first > prev); prev = kv.first; }
}

static void test_stress_unordered() {
    std::unordered_map<int,int> um;
    int model[2000];
    bool present[2000];
    for (int i = 0; i < 2000; ++i) { model[i] = 0; present[i] = false; }
    for (int i = 0; i < 50000; ++i) {
        int k = rng() % 2000;
        unsigned op = rng() % 3;
        if (op == 0) { um.erase(k); present[k] = false; }
        else { int val = (int)(rng() % 1000000); um[k] = val; model[k] = val; present[k] = true; }
    }
    size_t cnt = 0;
    for (int i = 0; i < 2000; ++i) if (present[i]) ++cnt;
    CHECK(um.size() == cnt);
    for (int i = 0; i < 2000; ++i) {
        auto it = um.find(i);
        if (present[i]) { CHECK(it != um.end()); CHECK(it->second == model[i]); }
        else CHECK(it == um.end());
    }
}

int main() {
    test_vector_string();
    test_map_string_vector();
    test_unordered_rehash();
    test_sort_strings();
    test_sort_pairs();
    test_list_sort_splice();
    test_deque_string();
    test_queue_stack();
    test_erase_if();
    test_stress_vector_string();
    test_stress_map();
    test_stress_unordered();
    return 0;
}
