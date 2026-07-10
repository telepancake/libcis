// Lean std::list smoke + stress test. Trap-on-failure (lean_test.h CHECK);
// standalone main(). Covers every major operation, non-trivial and move-only
// element types, the sizeof contract, reference/iterator stability, splice in
// all three forms across two lists with size checks, sort stability, the
// reverse+merge interplay, and a >=10k randomized run mirrored against a
// std::vector reference model with full-equality checks along the way.
#include <list>
#include <vector>
#include <string>
#include <utility>
#include "lean_test.h"

using std::list;

// ---- sizeof contract: sentinel (2 ptrs) + size_t, independent of T ----
// == 3 pointers exactly on both LP64 (24) and ILP32 (12).
static_assert(sizeof(list<char>)   == 3 * sizeof(void*), "lean list must be 3 pointers");
static_assert(sizeof(list<int>)    == 3 * sizeof(void*), "lean list must be 3 pointers");
static_assert(sizeof(list<double>) == 3 * sizeof(void*), "lean list must be 3 pointers");
static_assert(sizeof(list<std::string>) == 3 * sizeof(void*), "lean list must be 3 pointers");
static_assert(sizeof(list<int>) <= 3 * sizeof(void*), "lean list <= 3 pointers");

// ---- element types ----
struct Counted {
    static inline int live = 0;
    int v;
    Counted() : v(0) { ++live; }
    explicit Counted(int x) : v(x) { ++live; }
    Counted(const Counted& o) : v(o.v) { ++live; }
    Counted(Counted&& o) noexcept : v(o.v) { o.v = -1; ++live; }
    Counted& operator=(const Counted& o) { v = o.v; return *this; }
    Counted& operator=(Counted&& o) noexcept { v = o.v; o.v = -1; return *this; }
    ~Counted() { --live; }
    bool operator==(const Counted& o) const { return v == o.v; }
    bool operator<(const Counted& o) const { return v < o.v; }
};

struct MoveOnly {
    int v;
    explicit MoveOnly(int x = 0) : v(x) {}
    MoveOnly(MoveOnly&& o) noexcept : v(o.v) { o.v = -1; }
    MoveOnly& operator=(MoveOnly&& o) noexcept { v = o.v; o.v = -1; return *this; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    ~MoveOnly() {}
};

template<class L, class V>
static void check_seq(const L& l, const V& ref) {
    CHECK(l.size() == ref.size());
    CHECK(l.empty() == ref.empty());
    auto it = l.begin();
    for (size_t i = 0; i < ref.size(); ++i, ++it)
        CHECK(*it == ref[i]);
    CHECK(it == l.end());
    // reverse walk agrees too (exercises prev links)
    if (!ref.empty()) {
        auto rit = l.end();
        for (size_t i = ref.size(); i > 0; --i) {
            --rit;
            CHECK(*rit == ref[i - 1]);
        }
        CHECK(rit == l.begin());
    }
}

void test_push_pop_emplace() {
    list<int> l;
    CHECK(l.empty() && l.size() == 0);
    l.push_back(1);
    l.push_front(0);
    l.push_back(2);
    CHECK(l.front() == 0 && l.back() == 2 && l.size() == 3);
    CHECK(l.emplace_back(3) == 3);
    CHECK(l.emplace_front(-1) == -1);
    int expect[] = {-1, 0, 1, 2, 3};
    std::vector<int> ref(expect, expect + 5);
    check_seq(l, ref);
    l.pop_front();
    l.pop_back();
    CHECK(l.front() == 0 && l.back() == 2 && l.size() == 3);
}

void test_insert_erase() {
    list<int> l{1, 2, 3};
    auto it = l.begin(); ++it;              // points at 2
    auto r = l.insert(it, 10);              // 1 10 2 3
    CHECK(*r == 10);
    r = l.insert(l.end(), 3, 7);            // 1 10 2 3 7 7 7
    CHECK(*r == 7);
    int arr[] = {100, 101};
    l.insert(l.begin(), arr, arr + 2);      // 100 101 1 10 2 3 7 7 7
    l.insert(l.begin(), {200, 201});        // 200 201 100 101 1 10 2 3 7 7 7
    int expect[] = {200,201,100,101,1,10,2,3,7,7,7};
    std::vector<int> ref(expect, expect + 11);
    check_seq(l, ref);
    auto e = l.begin();
    std::advance(e, 4);                     // erase the '1'
    auto after = l.erase(e);
    CHECK(*after == 10);
    // erase range [begin, begin+2)
    auto f2 = l.begin(); auto l2 = l.begin(); std::advance(l2, 2);
    l.erase(f2, l2);
    CHECK(l.front() == 100);
}

void test_resize_assign_clear() {
    list<int> l;
    l.resize(5);                    // 0 0 0 0 0
    CHECK(l.size() == 5);
    for (int x : l) CHECK(x == 0);
    l.resize(8, 9);                 // 0 0 0 0 0 9 9 9
    CHECK(l.size() == 8 && l.back() == 9);
    l.resize(2);
    CHECK(l.size() == 2);
    l.assign(4, 3);
    { std::vector<int> ref{3,3,3,3}; check_seq(l, ref); }
    l.assign({5, 6, 7});
    { std::vector<int> ref{5,6,7}; check_seq(l, ref); }
    int src[] = {8, 9};
    l.assign(src, src + 2);
    { std::vector<int> ref{8,9}; check_seq(l, ref); }
    l.clear();
    CHECK(l.empty());
}

void test_copy_move_swap() {
    list<int> a{1, 2, 3, 4};
    list<int> b = a;                          // copy
    CHECK(a == b);
    b.push_back(5);
    CHECK(a != b);
    list<int> c = std::move(b);               // move
    CHECK(c.size() == 5 && b.empty());
    a.swap(c);
    CHECK(a.size() == 5 && c.size() == 4);
    c = a;                                     // copy assign (c smaller)
    CHECK(c == a);
    list<int> d{9};
    d = std::move(a);                          // move assign
    CHECK(d.size() == 5 && a.empty());
    // self-copy-assign is a no-op
    d = *&d;
    CHECK(d.size() == 5);
    std::swap(d, c);
    CHECK(!d.empty() && !c.empty());
}

void test_non_trivial_element_lifetimes() {
    CHECK(Counted::live == 0);
    {
        list<Counted> l;
        for (int i = 0; i < 6; ++i) l.emplace_back(i);
        CHECK(Counted::live == 6);
        list<Counted> m = l;                  // copy
        CHECK(Counted::live == 12);
        l.pop_front();
        l.pop_back();
        CHECK(Counted::live == 10);
        m.clear();
        CHECK(Counted::live == 4);
        l.insert(l.begin(), Counted(42));
        CHECK(l.front().v == 42);
        l.erase(l.begin());
        list<Counted> n = std::move(l);       // move: no new elements
        CHECK(n.size() == 4);
    }
    CHECK(Counted::live == 0);                 // destructor freed everything
}

void test_move_only_element() {
    list<MoveOnly> l;
    l.emplace_back(1);
    l.push_back(MoveOnly(2));
    l.emplace_front(0);
    l.insert(l.end(), MoveOnly(3));
    CHECK(l.size() == 4);
    int i = 0;
    for (auto& m : l) CHECK(m.v == i++);
    list<MoveOnly> m = std::move(l);           // move-construct
    CHECK(m.size() == 4 && l.empty());
    m.reverse();
    CHECK(m.front().v == 3 && m.back().v == 0);
    m.sort([](const MoveOnly& a, const MoveOnly& b) { return a.v < b.v; });
    CHECK(m.front().v == 0 && m.back().v == 3);
    m.pop_back();
    m.remove_if([](const MoveOnly& x) { return x.v == 1; });
    CHECK(m.size() == 2);                       // {0, 2}
    CHECK(m.front().v == 0 && m.back().v == 2);
}

void test_splice_all_forms() {
    // whole-list splice
    {
        list<int> a{1, 2, 3};
        list<int> b{4, 5, 6};
        auto it = a.begin(); ++it;             // before 2
        a.splice(it, b);                       // 1 4 5 6 2 3
        CHECK(a.size() == 6 && b.size() == 0 && b.empty());
        std::vector<int> ref{1,4,5,6,2,3};
        check_seq(a, ref);
    }
    // single-element splice
    {
        list<int> a{1, 2, 3};
        list<int> b{40, 50, 60};
        auto src = b.begin(); ++src;           // the 50
        a.splice(a.end(), b, src);             // a: 1 2 3 50   b: 40 60
        CHECK(a.size() == 4 && b.size() == 2);
        std::vector<int> ra{1,2,3,50}; check_seq(a, ra);
        std::vector<int> rb{40,60};   check_seq(b, rb);
    }
    // range splice across two lists (size bookkeeping)
    {
        list<int> a{1, 2, 3};
        list<int> b{7, 8, 9, 10};
        auto f = b.begin(); ++f;               // 8
        auto l = b.end();   --l;               // 10 (exclusive end)
        a.splice(a.begin(), b, f, l);          // move {8,9}: a: 8 9 1 2 3  b: 7 10
        CHECK(a.size() == 5 && b.size() == 2);
        std::vector<int> ra{8,9,1,2,3}; check_seq(a, ra);
        std::vector<int> rb{7,10};      check_seq(b, rb);
    }
    // same-list range splice (no size change, pure relink)
    {
        list<int> a{1, 2, 3, 4, 5};
        auto f = a.begin(); std::advance(f, 3);   // 4
        auto l = a.end();                          // exclusive
        a.splice(a.begin(), a, f, l);              // move {4,5} to front
        CHECK(a.size() == 5);
        std::vector<int> ref{4,5,1,2,3}; check_seq(a, ref);
    }
    // rvalue-list splice overloads compile and work
    {
        list<int> a{1};
        a.splice(a.end(), list<int>{2, 3});    // list&& whole-list splice overload
        CHECK(a.size() == 3);
        std::vector<int> ref{1, 2, 3}; check_seq(a, ref);
    }
}

void test_remove_unique_merge_reverse() {
    {
        list<int> l{1, 2, 2, 3, 2, 4, 4, 5};
        auto n = l.remove(2);
        CHECK(n == 3);
        std::vector<int> ref{1,3,4,4,5}; check_seq(l, ref);
    }
    {
        list<int> l{1, 2, 3, 4, 5, 6};
        auto n = l.remove_if([](int x) { return x % 2 == 0; });
        CHECK(n == 3);
        std::vector<int> ref{1,3,5}; check_seq(l, ref);
    }
    {
        list<int> l{1, 1, 2, 3, 3, 3, 4, 1};
        auto n = l.unique();
        CHECK(n == 3);
        std::vector<int> ref{1,2,3,4,1}; check_seq(l, ref);
    }
    {
        // reverse + merge interplay: reverse two descending lists into
        // ascending, then merge.
        list<int> a{9, 7, 5, 3, 1};
        list<int> b{10, 8, 6, 4, 2};
        a.reverse();                            // 1 3 5 7 9
        b.reverse();                            // 2 4 6 8 10
        a.merge(b);
        CHECK(b.empty() && a.size() == 10);
        std::vector<int> ref{1,2,3,4,5,6,7,8,9,10}; check_seq(a, ref);
    }
    {
        // merge with a custom comparator (both sorted descending)
        list<int> a{9, 6, 3};
        list<int> b{8, 5, 2};
        a.merge(b, [](int x, int y) { return x > y; });
        std::vector<int> ref{9,8,6,5,3,2}; check_seq(a, ref);
        CHECK(b.empty());
    }
    {
        // reverse of size 0 and 1 is a safe no-op
        list<int> e;
        e.reverse();
        CHECK(e.empty());
        list<int> one{42};
        one.reverse();
        CHECK(one.size() == 1 && one.front() == 42);
    }
}

void test_sort_stability() {
    // pair<key, tag>: sort by key only; equal keys must keep original order.
    using P = std::pair<int, int>;
    list<P> l;
    int keys[] = {3, 1, 2, 1, 3, 1, 2, 3, 2, 1};
    for (int i = 0; i < 10; ++i) l.emplace_back(keys[i], i);
    l.sort([](const P& a, const P& b) { return a.first < b.first; });
    CHECK(l.size() == 10);
    // verify sorted by key AND tags strictly increasing within each key group.
    auto it = l.begin();
    P prev = *it;
    ++it;
    for (; it != l.end(); ++it) {
        CHECK(prev.first <= it->first);
        if (prev.first == it->first)
            CHECK(prev.second < it->second);    // stability
        prev = *it;
    }
    // spot-check: all key==1 tags in original order 1,3,5,9
    std::vector<int> tags1;
    for (auto& p : l) if (p.first == 1) tags1.push_back(p.second);
    std::vector<int> exp1{1, 3, 5, 9};
    CHECK(tags1 == exp1);
}

void test_reference_iterator_stability() {
    // A node's address and referenced value survive every relinking op.
    list<int> l{10, 20, 30, 40, 50};
    auto it30 = l.begin(); std::advance(it30, 2);   // -> 30
    int* p30 = &*it30;
    int& r30 = *it30;
    CHECK(*p30 == 30 && r30 == 30);

    // insert / erase elsewhere: 30's node unaffected
    l.push_front(5);
    l.push_back(60);
    l.erase(l.begin());
    CHECK(&*it30 == p30 && *p30 == 30);

    // sort relinks but never moves values
    l.sort();
    CHECK(&*it30 == p30 && *p30 == 30);
    CHECK(r30 == 30);

    // reverse relinks but never moves values
    l.reverse();
    CHECK(&*it30 == p30 && *p30 == 30);

    // splice across lists preserves node identity
    list<int> other;
    other.splice(other.end(), l, it30);
    CHECK(&*it30 == p30 && *p30 == 30);
    CHECK(other.size() == 1 && other.front() == 30 && &other.front() == p30);

    // end() is stable across all ops (embedded sentinel)
    list<int> s{1, 2, 3};
    auto e = s.end();
    s.push_back(4); s.push_front(0); s.sort(); s.reverse();
    CHECK(s.end() == e);
}

void test_comparisons() {
    list<int> a{1, 2, 3};
    list<int> b{1, 2, 3};
    list<int> c{1, 2, 4};
    list<int> d{1, 2};
    CHECK(a == b);
    CHECK(a != c);
    CHECK((a <=> c) == std::strong_ordering::less);
    CHECK((c <=> a) == std::strong_ordering::greater);
    CHECK((a <=> b) == std::strong_ordering::equal);
    CHECK((d <=> a) == std::strong_ordering::less);   // prefix is less
    CHECK(a < c && c > a && d < a);
}

void test_erase_free_functions() {
    list<int> l{1, 2, 3, 2, 4, 2, 5};
    auto n = std::erase(l, 2);
    CHECK(n == 3);
    std::vector<int> ref{1,3,4,5}; check_seq(l, ref);
    auto m = std::erase_if(l, [](int x) { return x > 3; });
    CHECK(m == 2);
    std::vector<int> ref2{1,3}; check_seq(l, ref2);
}

void test_iterators_and_reverse_iterators() {
    list<int> l{1, 2, 3, 4, 5};
    // forward
    int sum = 0;
    for (auto it = l.cbegin(); it != l.cend(); ++it) sum += *it;
    CHECK(sum == 15);
    // reverse iterators
    std::vector<int> rev;
    for (auto it = l.rbegin(); it != l.rend(); ++it) rev.push_back(*it);
    std::vector<int> exp{5,4,3,2,1};
    CHECK(rev == exp);
    // const reverse
    const list<int>& cl = l;
    int s2 = 0;
    for (auto it = cl.crbegin(); it != cl.crend(); ++it) s2 += *it;
    CHECK(s2 == 15);
    // post/pre inc/dec
    auto it = l.begin();
    CHECK(*it++ == 1);
    CHECK(*it == 2);
    CHECK(*++it == 3);
    CHECK(*--it == 2);
    CHECK(*it-- == 2);
    CHECK(*it == 1);
    // iterator/const_iterator heterogeneous compare
    list<int>::iterator mi = l.begin();
    list<int>::const_iterator ci = l.begin();
    CHECK(mi == ci);
}

// ---------------------------------------------------------------------------
// Randomized stress: >=10k operations mirrored against a std::vector<int>,
// with a full-equality check on every iteration.
// ---------------------------------------------------------------------------
static unsigned rng_state = 0x9e3779b9u;
static unsigned rng() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

void test_stress_vs_reference() {
    list<int> l;
    std::vector<int> ref;
    const int OPS = 12000;
    for (int step = 0; step < OPS; ++step) {
        unsigned op = rng() % 12;
        int val = static_cast<int>(rng() % 100);
        switch (op) {
        case 0:  // push_back
            l.push_back(val); ref.push_back(val);
            break;
        case 1:  // push_front
            l.push_front(val); ref.insert(ref.begin(), val);
            break;
        case 2:  // pop_back
            if (!ref.empty()) { l.pop_back(); ref.pop_back(); }
            break;
        case 3:  // pop_front
            if (!ref.empty()) { l.pop_front(); ref.erase(ref.begin()); }
            break;
        case 4: { // insert at random position
            size_t pos = ref.empty() ? 0 : (rng() % (ref.size() + 1));
            auto it = l.begin(); std::advance(it, static_cast<long>(pos));
            l.insert(it, val);
            ref.insert(ref.begin() + static_cast<long>(pos), val);
            break;
        }
        case 5: { // erase at random position
            if (!ref.empty()) {
                size_t pos = rng() % ref.size();
                auto it = l.begin(); std::advance(it, static_cast<long>(pos));
                l.erase(it);
                ref.erase(ref.begin() + static_cast<long>(pos));
            }
            break;
        }
        case 6: { // insert a run of N
            unsigned n = rng() % 5;
            size_t pos = ref.empty() ? 0 : (rng() % (ref.size() + 1));
            auto it = l.begin(); std::advance(it, static_cast<long>(pos));
            l.insert(it, n, val);
            ref.insert(ref.begin() + static_cast<long>(pos), n, val);
            break;
        }
        case 7:  // sort both (stable; ints so just compare sequence)
            l.sort();
            std::sort(ref.begin(), ref.end());
            break;
        case 8:  // reverse
            l.reverse();
            std::reverse(ref.begin(), ref.end());
            break;
        case 9: { // remove a value
            l.remove(val);
            ref.erase(std::remove(ref.begin(), ref.end(), val), ref.end());
            break;
        }
        case 10: { // unique (needs sort first to be meaningful, but valid anytime)
            l.sort();
            std::sort(ref.begin(), ref.end());
            l.unique();
            ref.erase(std::unique(ref.begin(), ref.end()), ref.end());
            break;
        }
        case 11: { // resize
            size_t ns = rng() % 50;
            l.resize(ns, val);
            ref.resize(ns, val);
            break;
        }
        }
        // full equality every step
        CHECK(l.size() == ref.size());
        auto it = l.begin();
        for (size_t i = 0; i < ref.size(); ++i, ++it)
            CHECK(*it == ref[i]);
        CHECK(it == l.end());
    }
}

void test_stress_splice_two_lists() {
    // Mirror two lists a,b against two vectors; randomly splice between them.
    // Elements are Counted so the trailing live-count check is real: splice only
    // relinks nodes (never constructs/destroys a value), so after clearing both
    // lists every Counted made here must be gone.
    const long live_before = Counted::live;
    list<Counted> a, b;
    std::vector<int> ra, rb;
    const int OPS = 4000;
    for (int step = 0; step < OPS; ++step) {
        unsigned op = rng() % 6;
        int val = static_cast<int>(rng() % 1000);
        switch (op) {
        case 0: a.push_back(Counted(val)); ra.push_back(val); break;
        case 1: b.push_back(Counted(val)); rb.push_back(val); break;
        case 2: { // move whole b into a at front
            a.splice(a.begin(), b);
            ra.insert(ra.begin(), rb.begin(), rb.end());
            rb.clear();
            break;
        }
        case 3: { // single-element splice b->a
            if (!rb.empty()) {
                size_t pos = rng() % rb.size();
                auto it = b.begin(); std::advance(it, static_cast<long>(pos));
                a.splice(a.end(), b, it);
                ra.push_back(rb[pos]);
                rb.erase(rb.begin() + static_cast<long>(pos));
            }
            break;
        }
        case 4: { // range splice a->b
            if (!ra.empty()) {
                size_t i = rng() % ra.size();
                size_t j = i + (rng() % (ra.size() - i + 1));   // [i,j]
                auto f = a.begin(); std::advance(f, static_cast<long>(i));
                auto e = a.begin(); std::advance(e, static_cast<long>(j));
                b.splice(b.end(), a, f, e);
                rb.insert(rb.end(), ra.begin() + static_cast<long>(i),
                                    ra.begin() + static_cast<long>(j));
                ra.erase(ra.begin() + static_cast<long>(i),
                         ra.begin() + static_cast<long>(j));
            }
            break;
        }
        case 5: { // same-list range splice (move a block to front of a)
            if (ra.size() >= 2) {
                size_t i = 1 + rng() % (ra.size() - 1);
                auto f = a.begin(); std::advance(f, static_cast<long>(i));
                a.splice(a.begin(), a, f, a.end());
                std::vector<int> tail(ra.begin() + static_cast<long>(i), ra.end());
                ra.erase(ra.begin() + static_cast<long>(i), ra.end());
                ra.insert(ra.begin(), tail.begin(), tail.end());
            }
            break;
        }
        }
        CHECK(a.size() == ra.size());
        CHECK(b.size() == rb.size());
        { auto it = a.begin(); for (size_t i = 0; i < ra.size(); ++i, ++it) CHECK(it->v == ra[i]); CHECK(it == a.end()); }
        { auto it = b.begin(); for (size_t i = 0; i < rb.size(); ++i, ++it) CHECK(it->v == rb[i]); CHECK(it == b.end()); }
    }
    // Real leak/double-free invariant: clear both lists; every Counted created
    // in this test must now be destroyed (net live back to the entry value).
    a.clear();
    b.clear();
    CHECK(Counted::live == live_before);
}

int main() {
    test_push_pop_emplace();
    test_insert_erase();
    test_resize_assign_clear();
    test_copy_move_swap();
    test_non_trivial_element_lifetimes();
    test_move_only_element();
    test_splice_all_forms();
    test_remove_unique_merge_reverse();
    test_sort_stability();
    test_reference_iterator_stability();
    test_comparisons();
    test_erase_free_functions();
    test_iterators_and_reverse_iterators();
    test_stress_vs_reference();
    test_stress_splice_two_lists();
    return 0;
}
