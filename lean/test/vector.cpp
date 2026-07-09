// Lean vector<T> / vector<bool> smoke + stress test. Traps on failure.
#include <vector>
#include <string>
#include <iterator>
#include "lean_test.h"

using std::vector;
using std::string;

//===----------------------------------------------------------------------===//
// sizeof contract
//===----------------------------------------------------------------------===//
static_assert(sizeof(vector<int>)    == sizeof(void*), "vector<int> is one pointer");
static_assert(sizeof(vector<string>) == sizeof(void*), "vector<string> is one pointer");
static_assert(sizeof(vector<double>) == sizeof(void*), "vector<double> is one pointer");
static_assert(sizeof(vector<bool>)   == sizeof(void*), "vector<bool> is one pointer");

//===----------------------------------------------------------------------===//
// Test element types
//===----------------------------------------------------------------------===//

// Copyable, NON-trivially-relocatable, lifetime-counted: exercises the
// malloc-fresh + relocate-thunk + free path and the copy/destroy thunks.
struct Counted {
    static inline long live = 0;
    static inline long ctor = 0;
    int v;
    Counted() : v(0) { ++live; ++ctor; }
    explicit Counted(int x) : v(x) { ++live; ++ctor; }
    Counted(const Counted& o) : v(o.v) { ++live; ++ctor; }
    Counted(Counted&& o) noexcept : v(o.v) { o.v = -1; ++live; ++ctor; }
    Counted& operator=(const Counted& o) { v = o.v; return *this; }
    Counted& operator=(Counted&& o) noexcept { v = o.v; o.v = -1; return *this; }
    ~Counted() { --live; }
    friend bool operator==(const Counted& a, const Counted& b) { return a.v == b.v; }
    friend bool operator<(const Counted& a, const Counted& b) { return a.v < b.v; }
};

// Move-only, non-trivially-relocatable: exercises relocate without ever
// touching the copy thunk (which must remain uninstantiated / null).
struct MoveOnly {
    int* p;
    MoveOnly() : p(nullptr) {}
    explicit MoveOnly(int v) : p(new int(v)) {}
    MoveOnly(MoveOnly&& o) noexcept : p(o.p) { o.p = nullptr; }
    MoveOnly& operator=(MoveOnly&& o) noexcept {
        if (this != &o) { delete p; p = o.p; o.p = nullptr; }
        return *this;
    }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    ~MoveOnly() { delete p; }
    int val() const { return p ? *p : -1; }
};

//===----------------------------------------------------------------------===//
// Empty / sizeof-runtime invariants
//===----------------------------------------------------------------------===//
void test_empty_invariants() {
    vector<int> v;
    CHECK(v.data() == nullptr);
    CHECK(v.size() == 0);
    CHECK(v.capacity() == 0);
    CHECK(v.empty());
    CHECK(v.begin() == v.end());

    vector<int> w{};
    CHECK(w.begin() == w.end());
    CHECK(w.data() == nullptr);

    // moved-from is empty & null
    vector<int> a{1, 2, 3};
    vector<int> b(std::move(a));
    CHECK(a.data() == nullptr);
    CHECK(a.size() == 0);
    CHECK(b.size() == 3);
}

//===----------------------------------------------------------------------===//
// int basics
//===----------------------------------------------------------------------===//
void test_int_basic() {
    vector<int> v;
    for (int i = 0; i < 100; ++i)
        v.push_back(i);
    CHECK(v.size() == 100);
    CHECK(v.capacity() >= 100);
    for (int i = 0; i < 100; ++i) {
        CHECK(v[i] == i);
        CHECK(v.at(static_cast<size_t>(i)) == i);
    }
    CHECK(v.front() == 0);
    CHECK(v.back() == 99);
    // contiguity
    int* d = v.data();
    for (int i = 0; i < 100; ++i)
        CHECK(d[i] == i);

    v.pop_back();
    CHECK(v.size() == 99);
    CHECK(v.back() == 98);

    v.clear();
    CHECK(v.size() == 0);
    CHECK(v.empty());
    // capacity retained after clear
    CHECK(v.capacity() >= 99);

    // emplace_back returns reference
    vector<int> e;
    int& r = e.emplace_back(42);
    CHECK(r == 42);
    CHECK(&r == &e[0]);
}

//===----------------------------------------------------------------------===//
// reserve / capacity / shrink / iterator stability
//===----------------------------------------------------------------------===//
void test_reserve_shrink_stability() {
    vector<int> v;
    v.reserve(1000);
    CHECK(v.capacity() >= 1000);
    CHECK(v.size() == 0);
    int* base = v.data();
    // pushes within reserved capacity must not reallocate
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);
    CHECK(v.data() == base);        // no reallocation => pointer stable
    int* p0 = &v[0];
    int* p500 = &v[500];
    CHECK(*p0 == 0 && *p500 == 500);

    v.shrink_to_fit();
    CHECK(v.size() == 1000);
    for (int i = 0; i < 1000; ++i)
        CHECK(v[i] == i);

    // shrink to empty frees to null
    vector<int> w{1, 2, 3};
    w.clear();
    w.shrink_to_fit();
    CHECK(w.data() == nullptr);
    CHECK(w.capacity() == 0);
}

//===----------------------------------------------------------------------===//
// insert / erase (int)
//===----------------------------------------------------------------------===//
void test_insert_erase_int() {
    vector<int> v{0, 1, 2, 3, 4};
    // insert single at begin
    auto it = v.insert(v.begin(), -1);
    CHECK(*it == -1);
    CHECK(v[0] == -1 && v[1] == 0 && v[5] == 4);
    CHECK(v.size() == 6);

    // insert single at end
    v.insert(v.end(), 99);
    CHECK(v.back() == 99);

    // insert n copies in middle
    auto it2 = v.insert(v.begin() + 2, 3, 7);
    CHECK(*it2 == 7);
    CHECK(v[2] == 7 && v[3] == 7 && v[4] == 7);
    CHECK(v[5] == 1);

    // insert range
    int arr[] = {100, 200, 300};
    v.insert(v.begin(), arr, arr + 3);
    CHECK(v[0] == 100 && v[1] == 200 && v[2] == 300 && v[3] == -1);

    // erase single
    auto e = v.erase(v.begin());
    CHECK(*e == 200);
    CHECK(v[0] == 200);

    // erase range
    size_t before = v.size();
    v.erase(v.begin(), v.begin() + 2);
    CHECK(v.size() == before - 2);
    CHECK(v[0] == -1);

    // erase to empty
    v.erase(v.begin(), v.end());
    CHECK(v.empty());

    // insert initializer_list
    vector<int> z{1, 2};
    z.insert(z.begin() + 1, {8, 9, 10});
    CHECK(z.size() == 5);
    CHECK(z[0] == 1 && z[1] == 8 && z[2] == 9 && z[3] == 10 && z[4] == 2);
}

//===----------------------------------------------------------------------===//
// resize / assign / operator=
//===----------------------------------------------------------------------===//
void test_resize_assign() {
    vector<int> v;
    v.resize(5);
    CHECK(v.size() == 5);
    for (int x : v) CHECK(x == 0);
    v.resize(8, 7);
    CHECK(v.size() == 8);
    CHECK(v[5] == 7 && v[7] == 7);
    v.resize(3);
    CHECK(v.size() == 3);
    CHECK(v[0] == 0);

    v.assign(4, 9);
    CHECK(v.size() == 4);
    for (int x : v) CHECK(x == 9);

    int arr[] = {1, 2, 3};
    v.assign(arr, arr + 3);
    CHECK(v.size() == 3 && v[2] == 3);

    v.assign({5, 6, 7, 8});
    CHECK(v.size() == 4 && v[3] == 8);

    vector<int> a{1, 2, 3};
    vector<int> b;
    b = a;
    CHECK(b.size() == 3 && b[1] == 2);
    b = {9, 9};
    CHECK(b.size() == 2);
    vector<int> c;
    c = std::move(a);
    CHECK(c.size() == 3);
    CHECK(a.data() == nullptr);
}

//===----------------------------------------------------------------------===//
// ctors
//===----------------------------------------------------------------------===//
void test_ctors() {
    vector<int> a(5);
    CHECK(a.size() == 5);
    for (int x : a) CHECK(x == 0);

    vector<int> b(4, 7);
    CHECK(b.size() == 4 && b[3] == 7);

    int arr[] = {1, 2, 3, 4};
    vector<int> c(arr, arr + 4);
    CHECK(c.size() == 4 && c[2] == 3);

    vector<int> d{10, 20, 30};
    CHECK(d.size() == 3 && d[1] == 20);

    vector<int> e(c);
    CHECK(e.size() == 4 && e[3] == 4);
    CHECK(e.data() != c.data());   // deep copy

    // deduction guide
    vector deduced(arr, arr + 4);
    static_assert(std::is_same_v<decltype(deduced)::value_type, int>);
    CHECK(deduced.size() == 4);
}

//===----------------------------------------------------------------------===//
// std::string (non-trivial copy, trivially relocatable => memmove relocate)
//===----------------------------------------------------------------------===//
void test_string() {
    vector<string> v;
    for (int i = 0; i < 50; ++i)
        v.push_back(string("item-") + char('0' + (i % 10)));
    CHECK(v.size() == 50);
    CHECK(v[0] == "item-0");
    CHECK(v[9] == "item-9");

    v.insert(v.begin() + 5, "inserted");
    CHECK(v[5] == "inserted");
    CHECK(v.size() == 51);

    v.insert(v.begin(), 2, string("dup"));
    CHECK(v[0] == "dup" && v[1] == "dup");

    v.erase(v.begin(), v.begin() + 2);
    CHECK(v[5] == "inserted");

    v.resize(10);
    CHECK(v.size() == 10);
    v.resize(20, "pad");
    CHECK(v.size() == 20 && v[19] == "pad");

    vector<string> copy = v;
    CHECK(copy == v);
    copy[0] = "changed";
    CHECK(!(copy == v));

    // long strings (heap) to stress relocation of the string payloads
    vector<string> big;
    string longs(200, 'x');
    for (int i = 0; i < 40; ++i)
        big.push_back(longs);
    for (auto& s : big)
        CHECK(s.size() == 200);
}

//===----------------------------------------------------------------------===//
// Lifetime accounting (Counted): no leaks, exact live count
//===----------------------------------------------------------------------===//
void test_counted_lifetime() {
    CHECK(Counted::live == 0);
    {
        vector<Counted> v;
        for (int i = 0; i < 200; ++i)
            v.emplace_back(i);
        CHECK(Counted::live == 200);
        v.erase(v.begin() + 10, v.begin() + 20);
        CHECK(Counted::live == 190);
        CHECK(v.size() == 190);
        CHECK(v[10].v == 20);

        v.insert(v.begin() + 5, Counted(-5));
        CHECK(v[5].v == -5);
        CHECK(Counted::live == 191);

        v.resize(50);
        CHECK(Counted::live == 50);

        vector<Counted> w = v;         // copy
        CHECK(Counted::live == 100);
        w.clear();
        CHECK(Counted::live == 50);
    }
    CHECK(Counted::live == 0);         // all destroyed, no leaks
}

//===----------------------------------------------------------------------===//
// Move-only elements
//===----------------------------------------------------------------------===//
void test_move_only() {
    // copy thunk must NOT be instantiated for a move-only type
    CHECK(std::detail::lean_ops_for<MoveOnly>.copy == nullptr);
    CHECK(std::detail::lean_ops_for<MoveOnly>.relocate != nullptr);

    vector<MoveOnly> v;
    for (int i = 0; i < 100; ++i)
        v.push_back(MoveOnly(i));       // grows via relocate thunk
    CHECK(v.size() == 100);
    for (int i = 0; i < 100; ++i)
        CHECK(v[i].val() == i);

    v.emplace_back(1000);
    CHECK(v.back().val() == 1000);

    // erase from the middle (relocate shift)
    v.erase(v.begin() + 50);
    CHECK(v.size() == 100);
    CHECK(v[50].val() == 51);

    // insert (single) move
    v.insert(v.begin(), MoveOnly(-1));
    CHECK(v[0].val() == -1);
    CHECK(v[1].val() == 0);

    v.resize(10);                       // shrink destroys tail
    CHECK(v.size() == 10);
    CHECK(v[0].val() == -1);

    vector<MoveOnly> moved(std::move(v));
    CHECK(moved.size() == 10);
    CHECK(v.data() == nullptr);
}

//===----------------------------------------------------------------------===//
// Self-referential operations (standard requires these to work)
//===----------------------------------------------------------------------===//
void test_self_reference() {
    // push_back(v[0]) forcing a reallocation
    vector<int> v;
    v.push_back(7);
    for (int i = 0; i < 200; ++i)
        v.push_back(v[0]);              // aliases element 0 across many growths
    CHECK(v.size() == 201);
    for (int x : v) CHECK(x == 7);

    // push_back of the last element while at exact capacity
    vector<int> w;
    w.reserve(4);
    w.push_back(1);
    w.push_back(2);
    w.push_back(3);
    w.push_back(4);                     // now full
    CHECK(w.capacity() >= 4);
    while (w.size() < w.capacity()) w.push_back(0);   // fill to exact cap
    int lastval = w.back();
    w.push_back(w.back());              // aliasing element while growing
    CHECK(w.back() == lastval);

    // emplace(begin, v[k]) with growth
    vector<Counted> c;
    for (int i = 0; i < 8; ++i) c.emplace_back(i);
    while (c.size() < c.capacity()) c.emplace_back(0);
    Counted expect = c[2];
    c.emplace(c.begin(), c[2]);         // self ref, forces grow if full
    CHECK(c.front().v == expect.v);

    // insert(pos, x) where x is an element, no growth (in-place gap)
    vector<int> z{0, 1, 2, 3, 4, 5};
    z.reserve(20);
    z.insert(z.begin() + 1, z[4]);      // insert value of z[4]==4 at pos1
    CHECK(z[1] == 4);
    CHECK(z[0] == 0 && z[2] == 1);
}

//===----------------------------------------------------------------------===//
// Insert from own range (append a prefix of *this)
//===----------------------------------------------------------------------===//
void test_insert_own_range() {
    // with growth
    {
        vector<int> v{0, 1, 2, 3, 4};
        v.shrink_to_fit();              // make growth likely
        size_t k = 3;
        v.insert(v.end(), v.begin(), v.begin() + k);
        CHECK(v.size() == 8);
        CHECK(v[5] == 0 && v[6] == 1 && v[7] == 2);
        CHECK(v[0] == 0 && v[4] == 4);
    }
    // without growth (reserved room)
    {
        vector<int> v{0, 1, 2, 3, 4};
        v.reserve(32);
        v.insert(v.end(), v.begin(), v.begin() + 4);
        CHECK(v.size() == 9);
        CHECK(v[5] == 0 && v[6] == 1 && v[7] == 2 && v[8] == 3);
    }
    // non-trivial element self-append
    {
        vector<string> v{"a", "b", "c"};
        v.insert(v.end(), v.begin(), v.end());
        CHECK(v.size() == 6);
        CHECK(v[3] == "a" && v[4] == "b" && v[5] == "c");
    }
}

//===----------------------------------------------------------------------===//
// comparisons / swap
//===----------------------------------------------------------------------===//
void test_comparisons_swap() {
    vector<int> a{1, 2, 3};
    vector<int> b{1, 2, 3};
    vector<int> c{1, 2, 4};
    vector<int> d{1, 2};
    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(d < a);
    CHECK((a <=> b) == std::strong_ordering::equal);
    CHECK((a <=> c) == std::strong_ordering::less);
    CHECK((c <=> a) == std::strong_ordering::greater);

    a.swap(c);
    CHECK(a[2] == 4 && c[2] == 3);
    std::swap(a, c);
    CHECK(a[2] == 3 && c[2] == 4);
}

//===----------------------------------------------------------------------===//
// erase / erase_if free functions
//===----------------------------------------------------------------------===//
void test_erase_free() {
    vector<int> v{1, 2, 3, 2, 4, 2, 5};
    auto n = std::erase(v, 2);
    CHECK(n == 3);
    CHECK(v.size() == 4);
    CHECK(v[0] == 1 && v[1] == 3 && v[2] == 4 && v[3] == 5);

    auto m = std::erase_if(v, [](int x) { return x % 2 == 1; });
    CHECK(m == 3);
    CHECK(v.size() == 1 && v[0] == 4);
}

//===----------------------------------------------------------------------===//
// vector<bool>
//===----------------------------------------------------------------------===//
void test_vector_bool() {
    vector<bool> v;
    for (int i = 0; i < 200; ++i)
        v.push_back(i % 3 == 0);
    CHECK(v.size() == 200);
    for (int i = 0; i < 200; ++i)
        CHECK(v[i] == (i % 3 == 0));

    // proxy assignment
    v[1] = true;
    CHECK(v[1] == true);
    v[0] = false;
    CHECK(v[0] == false);
    CHECK(v.at(3) == (3 % 3 == 0));

    // reference::flip
    bool before = v[7];
    v[7].flip();
    CHECK(v[7] == !before);

    CHECK(v.front() == v[0]);
    CHECK(v.back() == v[199]);

    // iteration
    size_t count_true = 0;
    for (bool b : v)
        if (b) ++count_true;
    size_t manual = 0;
    for (int i = 0; i < 200; ++i)
        if (v[i]) ++manual;
    CHECK(count_true == manual);

    // flip whole vector
    vector<bool> f{true, false, true, false, true};
    f.flip();
    CHECK(f[0] == false && f[1] == true && f[2] == false && f[3] == true && f[4] == false);

    // resize
    vector<bool> r;
    r.resize(70, true);
    CHECK(r.size() == 70);
    for (size_t i = 0; i < 70; ++i) CHECK(r[i] == true);
    r.resize(10);
    CHECK(r.size() == 10);
    r.resize(100, false);
    CHECK(r[10] == false && r[99] == false);

    // copy / move / compare
    vector<bool> a{true, false, true};
    vector<bool> b = a;
    CHECK(a == b);
    b[1] = true;
    CHECK(!(a == b));
    CHECK(a < b);
    vector<bool> c(std::move(a));
    CHECK(c.size() == 3);

    // pop_back, clear
    vector<bool> p{true, true, false};
    p.pop_back();
    CHECK(p.size() == 2);
    p.clear();
    CHECK(p.empty());

    // ctors
    vector<bool> n(65, true);
    CHECK(n.size() == 65);
    CHECK(n[64] == true);
    bool src[] = {true, false, false, true};
    vector<bool> fromrange(src, src + 4);
    CHECK(fromrange.size() == 4 && fromrange[3] == true);
}

//===----------------------------------------------------------------------===//
// 10k+ randomized stress vs a plain-array reference model
//===----------------------------------------------------------------------===//
namespace {
struct Rng {
    unsigned long s;
    explicit Rng(unsigned long seed) : s(seed) {}
    unsigned long next() {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        return s;
    }
    size_t bounded(size_t n) { return n ? next() % n : 0; }
};
}

void test_stress() {
    constexpr size_t MAXN = 30000;
    static int model[MAXN];
    size_t msize = 0;

    vector<int> v;
    Rng rng(0x9e3779b97f4a7c15ULL);

    auto check_equal = [&]() {
        CHECK(v.size() == msize);
        for (size_t i = 0; i < msize; ++i)
            CHECK(v[i] == model[i]);
    };

    for (int iter = 0; iter < 12000; ++iter) {
        int op = static_cast<int>(rng.bounded(10));
        switch (op) {
        case 0: case 1: case 2: {              // push_back
            if (msize < MAXN) {
                int val = static_cast<int>(rng.next() & 0xffff);
                v.push_back(val);
                model[msize++] = val;
            }
            break;
        }
        case 3: {                              // pop_back
            if (msize > 0) {
                v.pop_back();
                --msize;
            }
            break;
        }
        case 4: {                              // insert single at random pos
            if (msize < MAXN) {
                size_t pos = rng.bounded(msize + 1);
                int val = static_cast<int>(rng.next() & 0xffff);
                v.insert(v.begin() + static_cast<ptrdiff_t>(pos), val);
                for (size_t i = msize; i > pos; --i)
                    model[i] = model[i - 1];
                model[pos] = val;
                ++msize;
            }
            break;
        }
        case 5: {                              // insert n copies
            if (msize + 8 < MAXN) {
                size_t pos = rng.bounded(msize + 1);
                size_t cnt = rng.bounded(6) + 1;
                int val = static_cast<int>(rng.next() & 0xffff);
                v.insert(v.begin() + static_cast<ptrdiff_t>(pos), cnt, val);
                for (size_t i = msize; i > pos; --i)
                    model[i + cnt - 1] = model[i - 1];
                for (size_t i = 0; i < cnt; ++i)
                    model[pos + i] = val;
                msize += cnt;
            }
            break;
        }
        case 6: {                              // erase single
            if (msize > 0) {
                size_t pos = rng.bounded(msize);
                v.erase(v.begin() + static_cast<ptrdiff_t>(pos));
                for (size_t i = pos; i + 1 < msize; ++i)
                    model[i] = model[i + 1];
                --msize;
            }
            break;
        }
        case 7: {                              // erase range
            if (msize > 0) {
                size_t a = rng.bounded(msize);
                size_t b = a + rng.bounded(msize - a + 1);
                v.erase(v.begin() + static_cast<ptrdiff_t>(a),
                        v.begin() + static_cast<ptrdiff_t>(b));
                size_t cnt = b - a;
                for (size_t i = b; i < msize; ++i)
                    model[i - cnt] = model[i];
                msize -= cnt;
            }
            break;
        }
        case 8: {                              // resize
            size_t ns = rng.bounded(2000);
            if (ns < MAXN) {
                int fill = static_cast<int>(rng.next() & 0xffff);
                v.resize(ns, fill);
                if (ns > msize)
                    for (size_t i = msize; i < ns; ++i) model[i] = fill;
                msize = ns;
            }
            break;
        }
        case 9: {                              // clear / assign occasionally
            if ((rng.next() & 1) == 0) {
                v.clear();
                msize = 0;
            } else {
                size_t ns = rng.bounded(500);
                int val = static_cast<int>(rng.next() & 0xffff);
                v.assign(ns, val);
                for (size_t i = 0; i < ns; ++i) model[i] = val;
                msize = ns;
            }
            break;
        }
        }
        // full mirror equality check every iteration
        check_equal();
    }
    CHECK(v.size() == msize);
}

int main() {
    test_empty_invariants();
    test_int_basic();
    test_reserve_shrink_stability();
    test_insert_erase_int();
    test_resize_assign();
    test_ctors();
    test_string();
    test_counted_lifetime();
    test_move_only();
    test_self_reference();
    test_insert_own_range();
    test_comparisons_swap();
    test_erase_free();
    test_vector_bool();
    test_stress();
    return 0;
}
