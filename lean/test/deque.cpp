// lean/test/deque.cpp — block-based deque: API, reference stability at both
// ends, middle insert/erase, non-trivial and move-only elements, the sizeof
// contract, queue/stack round-trips, and a 10k randomized stress vs a manual
// reference model. Failures trap (CHECK); a binary exiting 0 means all ran.
#include <deque>
#include <queue>
#include <stack>
#include <utility>
#include "lean_test.h"

using namespace std;

// --- sizeof / block-size contract ------------------------------------------
static_assert(sizeof(deque<int>) <= 3 * sizeof(void*), "deque control object must be <= 3 pointers");
static_assert(sizeof(deque<char>) <= 3 * sizeof(void*), "");
struct Big { char b[600]; };
static_assert(sizeof(deque<Big>) <= 3 * sizeof(void*), "");

// --------------------------------------------------------------------------
// Element types
// --------------------------------------------------------------------------
struct Counted {
    static inline long live = 0;
    long v;
    Counted() : v(0) { ++live; }
    explicit Counted(long x) : v(x) { ++live; }
    Counted(const Counted& o) : v(o.v) { ++live; }
    Counted(Counted&& o) noexcept : v(o.v) { o.v = -1; ++live; }
    Counted& operator=(const Counted& o) { v = o.v; return *this; }
    Counted& operator=(Counted&& o) noexcept { v = o.v; o.v = -1; return *this; }
    ~Counted() { --live; }
    bool operator==(const Counted& o) const { return v == o.v; }
};

struct MoveOnly {
    long v;
    explicit MoveOnly(long x = 0) : v(x) {}
    MoveOnly(MoveOnly&& o) noexcept : v(o.v) { o.v = -1; }
    MoveOnly& operator=(MoveOnly&& o) noexcept { v = o.v; o.v = -1; return *this; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
};

// --------------------------------------------------------------------------
// Tiny deterministic RNG
// --------------------------------------------------------------------------
struct Rng {
    unsigned long s = 0x243F6A8885A308D3ull;
    unsigned long next() {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        return s;
    }
    unsigned next_mod(unsigned m) { return static_cast<unsigned>(next() % m); }
};

// --------------------------------------------------------------------------
// Reference model: a contiguous int buffer with a front margin. Mirrors every
// deque operation the stress test performs.
// --------------------------------------------------------------------------
struct Ref {
    int* buf; size_t cap, lo, hi;
    void init(size_t c) { buf = static_cast<int*>(::malloc(c * sizeof(int))); cap = c; lo = hi = c / 2; }
    void done() { ::free(buf); }
    size_t size() const { return hi - lo; }
    int at(size_t i) const { return buf[lo + i]; }
    void push_back(int x) { buf[hi++] = x; }
    void push_front(int x) { buf[--lo] = x; }
    void pop_back() { --hi; }
    void pop_front() { ++lo; }
    void insert(size_t pos, int x) {
        for (size_t j = hi; j > lo + pos; --j) buf[j] = buf[j - 1];
        buf[lo + pos] = x; ++hi;
    }
    void erase(size_t pos) {
        for (size_t j = lo + pos; j + 1 < hi; ++j) buf[j] = buf[j + 1];
        --hi;
    }
};

template<class D, class R>
void check_equal(const D& d, const R& r) {
    CHECK(d.size() == r.size());
    for (size_t i = 0; i < r.size(); ++i)
        CHECK(d[i] == r.at(i));
}

// --------------------------------------------------------------------------
// Tests
// --------------------------------------------------------------------------

void test_block_size() {
    // be must equal the kernel's runtime formula.
    CHECK(std::detail::lean_deque_block_elems(sizeof(int)) == 512 / sizeof(int));
    CHECK(std::detail::lean_deque_block_elems(sizeof(char)) == 512);
    CHECK(std::detail::lean_deque_block_elems(600) == 1);
}

void test_basic_ends() {
    deque<int> d;
    CHECK(d.empty());
    for (int i = 0; i < 500; ++i) d.push_back(i);
    for (int i = 1; i <= 500; ++i) d.push_front(-i);
    CHECK(d.size() == 1000);
    for (int i = 0; i < 1000; ++i) CHECK(d[i] == i - 500);
    CHECK(d.front() == -500);
    CHECK(d.back() == 499);
    CHECK(d.at(500) == 0);
    // pop from both ends
    for (int i = 0; i < 250; ++i) { d.pop_front(); d.pop_back(); }
    CHECK(d.size() == 500);
    CHECK(d.front() == -250);
    CHECK(d.back() == 249);
}

void test_emplace_ends_return() {
    deque<int> d;
    int& a = d.emplace_back(7);
    int& b = d.emplace_front(9);
    CHECK(a == 7 && b == 9);
    CHECK(&a == &d.back());
    CHECK(&b == &d.front());
}

void test_at_traps_bounds() {
    // We cannot catch a trap; just exercise the in-range path thoroughly.
    deque<int> d;
    for (int i = 0; i < 300; ++i) d.push_back(i * 2);
    for (int i = 0; i < 300; ++i) CHECK(d.at(static_cast<size_t>(i)) == i * 2);
    const deque<int>& cd = d;
    CHECK(cd.at(0) == 0);
    CHECK(cd[299] == 598);
}

// Reference stability: element addresses survive thousands of pushes at BOTH
// ends. Blocks never move, so a given element keeps its address forever.
void test_reference_stability() {
    deque<long> d;
    const int N = 2000;
    for (int i = 0; i < N; ++i) d.push_back(i);
    long* addrs[N];
    for (int i = 0; i < N; ++i) addrs[i] = &d[i];

    const int M = 5000;
    for (int i = 0; i < M; ++i) { d.push_front(-1 - i); d.push_back(N + i); }
    // The original N elements now live at logical indices [M, M+N).
    for (int i = 0; i < N; ++i) {
        CHECK(&d[M + i] == addrs[i]);   // address unchanged
        CHECK(d[M + i] == i);           // value intact
    }
    // popping from the far ends must also not move the originals
    for (int i = 0; i < 1000; ++i) { d.pop_front(); d.pop_back(); }
    for (int i = 0; i < N; ++i) {
        CHECK(&d[M - 1000 + i] == addrs[i]);
        CHECK(d[M - 1000 + i] == i);
    }
}

// Alternating single push at front/back, crossing many block boundaries.
void test_alternating_growth() {
    deque<int> d;
    Ref r; r.init(80000);
    for (int i = 0; i < 6000; ++i) {
        if (i & 1) { d.push_back(i);  r.push_back(i); }
        else       { d.push_front(i); r.push_front(i); }
    }
    check_equal(d, r);
    // now drain alternately
    while (!d.empty()) {
        if (d.size() & 1) { d.pop_back();  r.pop_back(); }
        else              { d.pop_front(); r.pop_front(); }
        if ((d.size() % 777) == 0) check_equal(d, r);
    }
    CHECK(d.empty());
    r.done();
}

void test_middle_insert_erase() {
    deque<int> d;
    for (int i = 0; i < 20; ++i) d.push_back(i);         // 0..19
    // insert single near front and near back
    auto it = d.insert(d.begin() + 3, 100);
    CHECK(*it == 100);
    CHECK(d[3] == 100 && d[2] == 2 && d[4] == 3);
    it = d.insert(d.end() - 2, 200);
    CHECK(*it == 200);
    CHECK(d[d.size() - 3] == 200);
    // emplace
    d.emplace(d.begin() + 1, 300);
    CHECK(d[1] == 300);
    // n-copy insert
    d.insert(d.begin() + 5, 4, 7);
    for (int k = 0; k < 4; ++k) CHECK(d[5 + k] == 7);
    // range insert
    int src[3] = {51, 52, 53};
    d.insert(d.begin() + 2, src, src + 3);
    CHECK(d[2] == 51 && d[3] == 52 && d[4] == 53);
    // erase single
    size_t before = d.size();
    auto e = d.erase(d.begin() + 2);
    CHECK(*e == 52);
    CHECK(d.size() == before - 1);
    // erase range
    before = d.size();
    d.erase(d.begin() + 2, d.begin() + 6);
    CHECK(d.size() == before - 4);
}

// Middle insert/erase vs the reference, across block boundaries.
void test_middle_vs_ref() {
    deque<int> d;
    Ref r; r.init(20000);
    for (int i = 0; i < 500; ++i) { d.push_back(i); r.push_back(i); }
    Rng rng;
    for (int step = 0; step < 1500; ++step) {
        unsigned pos = d.empty() ? 0 : rng.next_mod(static_cast<unsigned>(d.size()) + 1);
        if ((rng.next() & 1) || d.empty()) {
            int val = static_cast<int>(rng.next_mod(100000));
            d.insert(d.begin() + pos, val);
            r.insert(pos, val);
        } else {
            pos = rng.next_mod(static_cast<unsigned>(d.size()));
            d.erase(d.begin() + pos);
            r.erase(pos);
        }
        if ((step % 137) == 0) check_equal(d, r);
    }
    check_equal(d, r);
    r.done();
}

void test_resize_assign_shrink() {
    deque<int> d;
    d.resize(100);
    CHECK(d.size() == 100);
    for (int i = 0; i < 100; ++i) CHECK(d[i] == 0);
    d.resize(50);
    CHECK(d.size() == 50);
    d.resize(300, 9);
    CHECK(d.size() == 300);
    CHECK(d[299] == 9 && d[50] == 9 && d[49] == 0);

    d.assign(10, 5);
    CHECK(d.size() == 10);
    for (int i = 0; i < 10; ++i) CHECK(d[i] == 5);
    int src[4] = {1, 2, 3, 4};
    d.assign(src, src + 4);
    CHECK(d.size() == 4 && d[0] == 1 && d[3] == 4);
    d.assign({7, 8, 9});
    CHECK(d.size() == 3 && d[2] == 9);

    // shrink_to_fit keeps content
    deque<int> big;
    for (int i = 0; i < 4000; ++i) big.push_back(i);
    for (int i = 0; i < 3900; ++i) big.pop_front();
    big.shrink_to_fit();
    CHECK(big.size() == 100);
    for (int i = 0; i < 100; ++i) CHECK(big[i] == 3900 + i);
    big.clear();
    big.shrink_to_fit();
    CHECK(big.empty());
    big.push_back(42);        // usable after shrink-to-empty
    CHECK(big[0] == 42);
}

void test_compare_and_erase_if() {
    deque<int> a{1, 2, 3, 4, 5};
    deque<int> b{1, 2, 3, 4, 5};
    deque<int> c{1, 2, 3, 4, 6};
    CHECK(a == b);
    CHECK(!(a == c));
    CHECK(a < c);
    CHECK(c > a);
    CHECK((a <=> b) == 0);

    auto removed = erase(a, 3);
    CHECK(removed == 1);
    CHECK(a.size() == 4 && a[2] == 4);

    deque<int> e{1, 2, 3, 4, 5, 6, 7, 8};
    auto n = erase_if(e, [](int x) { return x % 2 == 0; });
    CHECK(n == 4);
    CHECK(e.size() == 4 && e[0] == 1 && e[3] == 7);
}

void test_copy_move_swap() {
    deque<int> a;
    for (int i = 0; i < 300; ++i) a.push_back(i);
    deque<int> b = a;                 // copy
    CHECK(b == a);
    deque<int> c = std::move(a);      // move
    CHECK(c.size() == 300 && a.empty());
    CHECK(c[299] == 299);
    deque<int> d;
    d = c;                            // copy assign
    CHECK(d == c);
    deque<int> e;
    e = std::move(c);                 // move assign
    CHECK(e.size() == 300 && c.empty());
    deque<int> f{9, 8, 7};
    e.swap(f);
    CHECK(e.size() == 3 && e[0] == 9);
    CHECK(f.size() == 300 && f[299] == 299);
    swap(e, f);
    CHECK(e.size() == 300 && f.size() == 3);
}

void test_iterators() {
    deque<int> d;
    for (int i = 0; i < 1000; ++i) d.push_back(i);
    long sum = 0;
    for (int x : d) sum += x;
    CHECK(sum == 999L * 1000 / 2);
    // reverse
    long rsum = 0; int last = 1000;
    for (auto it = d.rbegin(); it != d.rend(); ++it) { CHECK(*it == --last); rsum += *it; }
    CHECK(rsum == sum);
    // random access arithmetic
    auto b = d.begin();
    CHECK(*(b + 500) == 500);
    CHECK(b[123] == 123);
    CHECK((d.end() - d.begin()) == 1000);
    CHECK(d.end() - 1 > d.begin());
    // const iterator conversion
    const deque<int>& cd = d;
    auto cit = cd.begin();
    CHECK(*cit == 0);
    deque<int>::const_iterator conv = d.begin();  // non-const -> const
    CHECK(*conv == 0);
    // mutation through iterator
    for (auto it = d.begin(); it != d.end(); ++it) *it += 1;
    CHECK(d[0] == 1 && d[999] == 1000);
}

void test_nontrivial_element() {
    CHECK(Counted::live == 0);
    {
        deque<Counted> d;
        for (int i = 0; i < 1000; ++i) d.emplace_back(i);
        for (int i = 1; i <= 1000; ++i) d.emplace_front(Counted(-i));
        CHECK(d.size() == 2000);
        CHECK(Counted::live == 2000);
        for (int i = 0; i < 2000; ++i) CHECK(d[i].v == i - 1000);
        // middle insert/erase with non-trivial destructors
        d.insert(d.begin() + 500, Counted(12345));
        CHECK(d[500].v == 12345);
        CHECK(Counted::live == 2001);
        d.erase(d.begin() + 500);
        CHECK(Counted::live == 2000);
        d.resize(100);
        CHECK(Counted::live == 100);
        d.clear();
        CHECK(Counted::live == 0);
        for (int i = 0; i < 50; ++i) d.push_back(Counted(i));  // reuse after clear
        CHECK(Counted::live == 50);
    }
    CHECK(Counted::live == 0);   // destructor released everything
}

void test_move_only_element() {
    deque<MoveOnly> d;
    for (int i = 0; i < 500; ++i) d.emplace_back(i);
    for (int i = 1; i <= 500; ++i) d.push_front(MoveOnly(-i));
    CHECK(d.size() == 1000);
    for (int i = 0; i < 1000; ++i) CHECK(d[i].v == i - 500);
    // move-only insert/erase in the middle (emplace + shifting via move-assign)
    d.emplace(d.begin() + 250, 77777);
    CHECK(d[250].v == 77777);
    CHECK(d[251].v == 250 - 500);   // element that was at 250 shifted to 251
    d.erase(d.begin() + 250);       // insert-then-erase at 250 restores the original
    CHECK(d[250].v == 250 - 500);
    d.pop_front(); d.pop_back();
    CHECK(d.size() == 998);
    // move the whole container
    deque<MoveOnly> e = std::move(d);
    CHECK(e.size() == 998 && d.empty());
}

void test_queue_stack() {
    queue<int> q;
    CHECK(q.empty());
    for (int i = 0; i < 1000; ++i) q.push(i);
    CHECK(q.size() == 1000);
    CHECK(q.front() == 0);
    CHECK(q.back() == 999);
    for (int i = 0; i < 1000; ++i) { CHECK(q.front() == i); q.pop(); }
    CHECK(q.empty());
    q.emplace(5);
    CHECK(q.front() == 5 && q.back() == 5);

    stack<int> s;
    CHECK(s.empty());
    for (int i = 0; i < 1000; ++i) s.push(i);
    CHECK(s.size() == 1000);
    CHECK(s.top() == 999);
    for (int i = 999; i >= 0; --i) { CHECK(s.top() == i); s.pop(); }
    CHECK(s.empty());
    s.emplace(3);
    CHECK(s.top() == 3);
}

// 10k randomized ops mirrored against the reference model, with full equality
// checks along the way.
void test_stress_vs_reference() {
    deque<int> d;
    Ref r; r.init(120000);
    Rng rng;
    for (int step = 0; step < 12000; ++step) {
        unsigned op = rng.next_mod(10);
        switch (op) {
        case 0: case 1: {                     // push_back
            int v = static_cast<int>(rng.next());
            d.push_back(v); r.push_back(v);
            break;
        }
        case 2: case 3: {                     // push_front
            int v = static_cast<int>(rng.next());
            d.push_front(v); r.push_front(v);
            break;
        }
        case 4:                               // pop_back
            if (!d.empty()) { d.pop_back(); r.pop_back(); }
            break;
        case 5:                               // pop_front
            if (!d.empty()) { d.pop_front(); r.pop_front(); }
            break;
        case 6: {                             // middle insert
            unsigned pos = rng.next_mod(static_cast<unsigned>(d.size()) + 1);
            int v = static_cast<int>(rng.next());
            d.insert(d.begin() + pos, v); r.insert(pos, v);
            break;
        }
        case 7:                               // middle erase
            if (!d.empty()) {
                unsigned pos = rng.next_mod(static_cast<unsigned>(d.size()));
                d.erase(d.begin() + pos); r.erase(pos);
            }
            break;
        case 8:                               // write through operator[]
            if (!d.empty()) {
                unsigned pos = rng.next_mod(static_cast<unsigned>(d.size()));
                int v = static_cast<int>(rng.next());
                d[pos] = v; r.buf[r.lo + pos] = v;
            }
            break;
        case 9:                               // occasional bulk drain/refill
            if (d.size() > 20) { d.pop_front(); r.pop_front(); }
            break;
        }
        if ((step % 200) == 0) check_equal(d, r);
    }
    check_equal(d, r);
    // drain fully, checking every so often
    while (!d.empty()) {
        d.pop_back(); r.pop_back();
        if ((d.size() % 500) == 0) check_equal(d, r);
    }
    CHECK(d.empty());
    r.done();
}

// Regression: pop_front()/pop_back() must NOT invalidate iterators/references
// to surviving elements ([deque.modifiers]). The lean iterator stores an
// ABSOLUTE index, so an iterator taken before a pop still denotes the same
// element (and address) afterward. (Previously the index was off_-relative, so
// pop_front silently repointed every live iterator — fixed.)
static void test_pop_iterator_stability() {
    deque<int> d;
    for (int i = 0; i < 4098; ++i) d.push_back(i);
    while (d.size() > 2) {
        deque<int>::iterator it1 = d.begin() + 1;
        deque<int>::iterator it2 = d.end() - 1;
        int  v1 = *it1;  int* a1 = &*it1;
        int  v2 = *it2;  int* a2 = &*it2;
        d.pop_front();
        CHECK(it1 == d.begin());           // same element, now the front
        CHECK(*it1 == v1 && &*it1 == a1);  // reference/address preserved
        CHECK(it2 == d.end() - 1);         // back untouched by pop_front
        CHECK(*it2 == v2 && &*it2 == a2);
        d.pop_back();                      // pop_back must not move the front
        CHECK(*d.begin() == v1 && &*d.begin() == a1);
    }
}

int main() {
    test_pop_iterator_stability();
    test_block_size();
    test_basic_ends();
    test_emplace_ends_return();
    test_at_traps_bounds();
    test_reference_stability();
    test_alternating_growth();
    test_middle_insert_erase();
    test_middle_vs_ref();
    test_resize_assign_shrink();
    test_compare_and_erase_if();
    test_copy_move_swap();
    test_iterators();
    test_nontrivial_element();
    test_move_only_element();
    test_queue_stack();
    test_stress_vs_reference();
    return 0;
}
