// Lean unordered_map / unordered_set self-test. Failures trap (CHECK); no
// printing. Covers every major operation, non-trivial and move-only element
// types, the sizeof contract, reference stability across rehash, the cached-
// hash rehash guarantee (a counting hasher), string keys, and >=10k randomized
// stress runs mirrored against a plain-array reference model.
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>
#include <utility>
#include <cstdint>
#include <cstddef>
#include <limits>
#include "lean_test.h"

using std::unordered_map;
using std::unordered_set;
using std::string;

// ---------------------------------------------------------------------------
// sizeof contract (also asserted inside the headers for the default types)
// ---------------------------------------------------------------------------
static_assert(sizeof(unordered_map<int, int>) == sizeof(void*), "map is one pointer");
static_assert(sizeof(unordered_set<int>) == sizeof(void*), "set is one pointer");
static_assert(sizeof(unordered_map<string, string>) == sizeof(void*), "map is one pointer");
static_assert(sizeof(unordered_set<string>) == sizeof(void*), "set is one pointer");

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------
struct Rng {
    uint64_t s;
    explicit Rng(uint64_t seed) : s(seed ? seed : 0x9e3779b97f4a7c15ull) {}
    uint64_t next() {
        s ^= s << 13;
        s ^= s >> 7;
        s ^= s << 17;
        return s;
    }
    uint32_t below(uint32_t n) { return uint32_t(next() % n); }
};

// counts live instances so we can prove destructors run on erase/clear/destroy
struct Counted {
    static inline int live = 0;
    int v;
    Counted() : v(0) { ++live; }
    explicit Counted(int x) : v(x) { ++live; }
    Counted(const Counted& o) : v(o.v) { ++live; }
    Counted(Counted&& o) noexcept : v(o.v) { ++live; }
    Counted& operator=(const Counted& o) { v = o.v; return *this; }
    Counted& operator=(Counted&& o) noexcept { v = o.v; return *this; }
    ~Counted() { --live; }
    bool operator==(const Counted& o) const { return v == o.v; }
};

struct MoveOnly {
    int v;
    MoveOnly() : v(0) {}
    explicit MoveOnly(int x) : v(x) {}
    MoveOnly(MoveOnly&& o) noexcept : v(o.v) { o.v = -1; }
    MoveOnly& operator=(MoveOnly&& o) noexcept {
        v = o.v;
        o.v = -1;
        return *this;
    }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    bool operator==(const MoveOnly& o) const { return v == o.v; }
};
struct MOHash {
    size_t operator()(const MoveOnly& m) const { return std::hash<int>()(m.v); }
};
struct MOEq {
    bool operator()(const MoveOnly& a, const MoveOnly& b) const { return a.v == b.v; }
};

static bool is_pow2(size_t n) { return n != 0 && (n & (n - 1)) == 0; }

// ---------------------------------------------------------------------------
// map: core operations
// ---------------------------------------------------------------------------
void test_map_basic() {
    unordered_map<int, int> m;
    CHECK(m.empty());
    CHECK(m.size() == 0);
    CHECK(m.bucket_count() == 0);        // null control block == empty
    CHECK(m.begin() == m.end());
    CHECK(m.find(1) == m.end());
    CHECK(m.count(1) == 0);
    CHECK(!m.contains(1));

    // operator[] inserts default
    m[10] = 100;
    CHECK(m.size() == 1);
    CHECK(m[10] == 100);
    CHECK(m.at(10) == 100);
    CHECK(m.contains(10));
    CHECK(m.count(10) == 1);

    // insert pair, duplicate rejected
    std::pair<int, int> p(20, 200);
    auto r1 = m.insert(p);
    CHECK(r1.second && r1.first->second == 200);
    auto r2 = m.insert(std::pair<int, int>(20, 999));
    CHECK(!r2.second && r2.first->second == 200);

    // emplace
    auto r3 = m.emplace(30, 300);
    CHECK(r3.second && r3.first->second == 300);
    auto r4 = m.emplace(30, 111);
    CHECK(!r4.second);

    // try_emplace: no overwrite
    auto r5 = m.try_emplace(30, 555);
    CHECK(!r5.second && m.at(30) == 300);
    auto r6 = m.try_emplace(40, 400);
    CHECK(r6.second && m.at(40) == 400);

    // insert_or_assign: overwrite
    auto r7 = m.insert_or_assign(40, 444);
    CHECK(!r7.second && m.at(40) == 444);
    auto r8 = m.insert_or_assign(50, 500);
    CHECK(r8.second && m.at(50) == 500);

    // equal_range
    auto er = m.equal_range(30);
    CHECK(er.first != er.second);
    CHECK(er.first->first == 30);
    auto er2 = er.first;
    ++er2;
    CHECK(er2 == er.second);
    auto er3 = m.equal_range(9999);
    CHECK(er3.first == er3.second && er3.first == m.end());

    // erase by key / iterator
    CHECK(m.erase(10) == 1);
    CHECK(m.erase(10) == 0);
    auto it = m.find(20);
    CHECK(it != m.end());
    auto nxt = m.erase(it);
    CHECK(m.find(20) == m.end());
    (void)nxt;

    size_t before = m.size();
    m.clear();
    CHECK(m.size() == 0 && m.empty());
    CHECK(before > 0);

    // at() on missing key traps — not exercised (would abort); covered by
    // contains()/find() instead.
}

// ---------------------------------------------------------------------------
// set: core operations
// ---------------------------------------------------------------------------
void test_set_basic() {
    unordered_set<int> s;
    CHECK(s.empty() && s.bucket_count() == 0 && s.begin() == s.end());

    auto r1 = s.insert(5);
    CHECK(r1.second && *r1.first == 5);
    auto r2 = s.insert(5);
    CHECK(!r2.second);
    s.insert(7);
    s.emplace(9);
    CHECK(s.size() == 3);
    CHECK(s.count(5) == 1 && s.count(6) == 0);
    CHECK(s.contains(7) && !s.contains(8));

    auto er = s.equal_range(9);
    CHECK(er.first != er.second && *er.first == 9);

    CHECK(s.erase(5) == 1);
    CHECK(s.erase(5) == 0);
    auto it = s.find(7);
    CHECK(it != s.end());
    s.erase(it);
    CHECK(!s.contains(7));
    CHECK(s.size() == 1);
    s.clear();
    CHECK(s.empty());
}

// ---------------------------------------------------------------------------
// iterator walk visits exactly size() elements
// ---------------------------------------------------------------------------
void test_iterator_walk() {
    unordered_map<int, int> m;
    for (int i = 0; i < 500; ++i)
        m[i] = i;
    size_t n = 0;
    for (auto it = m.begin(); it != m.end(); ++it) {
        CHECK(it->first == it->second);
        ++n;
    }
    CHECK(n == m.size());
    CHECK(n == 500);

    // erase half, walk again
    for (int i = 0; i < 500; i += 2)
        m.erase(i);
    n = 0;
    for (const auto& kv : m) {
        CHECK((kv.first & 1) == 1);
        ++n;
    }
    CHECK(n == m.size() && n == 250);
}

// ---------------------------------------------------------------------------
// rehash / growth: force many growths, verify every key present, size exact,
// bucket_count is a growing power of two
// ---------------------------------------------------------------------------
void test_rehash_growth() {
    unordered_map<int, int> m;
    const int N = 20000;
    size_t last_bc = 0;
    int growths = 0;
    for (int i = 0; i < N; ++i) {
        m[i] = i + 7;
        if (m.bucket_count() != last_bc) {
            ++growths;
            last_bc = m.bucket_count();
            CHECK(is_pow2(last_bc));
        }
        // load factor never exceeds the max
        CHECK(m.load_factor() <= m.max_load_factor() + 1e-6f);
    }
    CHECK(growths >= 5); // definitely grew several times
    CHECK(m.size() == size_t(N));
    for (int i = 0; i < N; ++i) {
        auto it = m.find(i);
        CHECK(it != m.end() && it->second == i + 7);
    }

    // explicit rehash to a big count keeps all keys and stays pow2
    m.rehash(1 << 20);
    CHECK(is_pow2(m.bucket_count()));
    CHECK(m.bucket_count() >= (1u << 20));
    CHECK(m.size() == size_t(N));
    for (int i = 0; i < N; ++i)
        CHECK(m.find(i) != m.end());

    // rehash(0) must not drop below what is needed to hold size
    m.rehash(0);
    CHECK(m.size() == size_t(N));
    CHECK(m.load_factor() <= m.max_load_factor() + 1e-6f);
    for (int i = 0; i < N; ++i)
        CHECK(m.find(i) != m.end());
}

// ---------------------------------------------------------------------------
// cached-hash rehash never re-invokes Hash (counting hasher)
// ---------------------------------------------------------------------------
struct CountingHash {
    static inline long calls = 0;
    size_t operator()(int x) const {
        ++calls;
        return std::hash<int>()(x);
    }
};

void test_cached_hash_no_recompute() {
    CountingHash::calls = 0;
    unordered_map<int, int, CountingHash> m;
    const int N = 5000;
    for (int i = 0; i < N; ++i)
        m.emplace(i, i); // one distinct key each -> exactly one hash call each
    long after_inserts = CountingHash::calls;
    CHECK(after_inserts == N);

    // A rehash redistributes by cached hash and must call Hash zero times.
    m.rehash(1 << 18);
    CHECK(CountingHash::calls == after_inserts);
    m.reserve(200000);
    CHECK(CountingHash::calls == after_inserts);
    m.rehash(0);
    CHECK(CountingHash::calls == after_inserts);

    // Every key still present after those rehashes.
    long before_find = CountingHash::calls;
    for (int i = 0; i < N; ++i)
        CHECK(m.find(i) != m.end());
    CHECK(CountingHash::calls == before_find + N); // find hashes once per call
}

// ---------------------------------------------------------------------------
// reference / pointer stability across rehash (node-based container)
// ---------------------------------------------------------------------------
void test_reference_stability() {
    unordered_map<int, int> m;
    m.reserve(4);
    for (int i = 0; i < 8; ++i)
        m[i] = i * 10;
    int* p3 = &m.at(3);
    int* p5 = &m.at(5);
    size_t bc0 = m.bucket_count();

    // force many rehashes
    for (int i = 8; i < 4000; ++i)
        m[i] = i * 10;
    CHECK(m.bucket_count() > bc0);

    // addresses of existing elements are unchanged; values intact
    CHECK(&m.at(3) == p3);
    CHECK(&m.at(5) == p5);
    CHECK(*p3 == 30 && *p5 == 50);

    // iterators to non-erased elements survive erasure of another element
    auto it7 = m.find(7);
    int* p7 = &it7->second;
    m.erase(3);
    CHECK(it7->second == 70);
    CHECK(&it7->second == p7);
}

// ---------------------------------------------------------------------------
// string keys and values (non-trivial construction/destruction)
// ---------------------------------------------------------------------------
void test_string_keys() {
    unordered_map<string, int> m;
    m["alpha"] = 1;
    m["beta"] = 2;
    m["gamma"] = 3;
    m["alpha"] += 10;
    CHECK(m.size() == 3);
    CHECK(m.at("alpha") == 11);
    CHECK(m.contains("beta"));
    CHECK(m.find("zzz") == m.end());
    CHECK(m.erase("gamma") == 1);
    CHECK(m.size() == 2);

    unordered_set<string> s;
    for (int i = 0; i < 1000; ++i)
        s.insert(string("k") + std::to_string(i));
    CHECK(s.size() == 1000);
    for (int i = 0; i < 1000; ++i)
        CHECK(s.contains(string("k") + std::to_string(i)));
    CHECK(!s.contains("nope"));
    // duplicate string insert rejected
    auto r = s.insert(string("k500"));
    CHECK(!r.second && s.size() == 1000);
}

// ---------------------------------------------------------------------------
// non-trivial destruction: live count returns to zero on erase/clear/destroy
// ---------------------------------------------------------------------------
void test_nontrivial_destruction() {
    CHECK(Counted::live == 0);
    {
        unordered_map<int, Counted> m;
        for (int i = 0; i < 300; ++i)
            m.emplace(i, Counted(i));
        CHECK(Counted::live == 300);
        for (int i = 0; i < 100; ++i)
            m.erase(i);
        CHECK(Counted::live == 200);
        m.clear();
        CHECK(Counted::live == 0);
        for (int i = 0; i < 50; ++i)
            m[i] = Counted(i);
        CHECK(Counted::live == 50);
        // destructor of m runs here
    }
    CHECK(Counted::live == 0);
}

// ---------------------------------------------------------------------------
// move-only mapped type
// ---------------------------------------------------------------------------
void test_move_only_mapped() {
    unordered_map<int, MoveOnly> m;
    m[1] = MoveOnly(11);           // default-construct then move-assign
    m.try_emplace(2, 22);          // construct in place
    m.emplace(3, MoveOnly(33));    // move a temporary
    m.insert_or_assign(1, MoveOnly(111));
    m.insert(std::pair<const int, MoveOnly>(4, MoveOnly(44)));
    CHECK(m.size() == 4);
    CHECK(m.at(1).v == 111);
    CHECK(m.at(2).v == 22);
    CHECK(m.at(3).v == 33);
    CHECK(m.at(4).v == 44);

    // force rehash: values must survive (moved as nodes, not reconstructed)
    for (int i = 100; i < 3000; ++i)
        m.try_emplace(i, i);
    CHECK(m.at(1).v == 111 && m.at(3).v == 33);
    CHECK(m.at(2500).v == 2500);

    m.erase(2);
    CHECK(m.find(2) == m.end());

    // move the whole map
    unordered_map<int, MoveOnly> m2(std::move(m));
    CHECK(m2.at(1).v == 111);
    CHECK(m.size() == 0); // moved-from is empty (null control block)
}

// ---------------------------------------------------------------------------
// move-only set element
// ---------------------------------------------------------------------------
void test_move_only_set() {
    unordered_set<MoveOnly, MOHash, MOEq> s;
    s.insert(MoveOnly(1));
    s.emplace(2);
    s.insert(MoveOnly(3));
    auto dup = s.insert(MoveOnly(2));
    CHECK(!dup.second);
    CHECK(s.size() == 3);
    CHECK(s.contains(MoveOnly(1)));
    CHECK(!s.contains(MoveOnly(9)));

    for (int i = 10; i < 2000; ++i)
        s.emplace(i);
    CHECK(s.size() == 3 + 1990);
    CHECK(s.contains(MoveOnly(1500)));
    s.erase(MoveOnly(1));
    CHECK(!s.contains(MoveOnly(1)));
}

// ---------------------------------------------------------------------------
// copy / move / swap / operator==
// ---------------------------------------------------------------------------
void test_copy_move_swap() {
    unordered_map<int, int> a;
    for (int i = 0; i < 200; ++i)
        a[i] = i * 3;

    unordered_map<int, int> b(a); // copy ctor
    CHECK(b.size() == a.size());
    CHECK(a == b);
    for (int i = 0; i < 200; ++i)
        CHECK(b.at(i) == i * 3);

    b[0] = -1;
    CHECK(!(a == b));  // independent storage
    CHECK(a.at(0) == 0);

    unordered_map<int, int> c;
    c = a; // copy assign
    CHECK(c == a);
    c[500] = 5;
    CHECK(c.size() == a.size() + 1);

    unordered_map<int, int> d(std::move(a)); // move ctor
    CHECK(d.size() == 200 && a.size() == 0);
    CHECK(d.at(199) == 199 * 3);

    unordered_map<int, int> e;
    e = std::move(d); // move assign
    CHECK(e.size() == 200 && d.size() == 0);

    // swap
    unordered_map<int, int> x, y;
    x[1] = 1;
    x[2] = 2;
    y[9] = 9;
    x.swap(y);
    CHECK(x.size() == 1 && x.at(9) == 9);
    CHECK(y.size() == 2 && y.at(1) == 1 && y.at(2) == 2);
    std::swap(x, y);
    CHECK(x.size() == 2 && y.size() == 1);

    // set operator==
    unordered_set<int> s1, s2;
    for (int i = 0; i < 50; ++i) {
        s1.insert(i);
        s2.insert(49 - i);
    }
    CHECK(s1 == s2); // order-independent
    s2.insert(1000);
    CHECK(!(s1 == s2));
}

// ---------------------------------------------------------------------------
// max_load_factor / reserve / load_factor
// ---------------------------------------------------------------------------
void test_load_factor() {
    // set max_load_factor on an empty container (allocates a minimal block)
    unordered_map<int, int> m;
    m.max_load_factor(0.5f);
    CHECK(m.max_load_factor() == 0.5f);
    CHECK(m.bucket_count() >= 2);

    for (int i = 0; i < 1000; ++i)
        m[i] = i;
    CHECK(m.load_factor() <= 0.5f + 1e-6f);
    CHECK(m.size() == 1000);

    // reserve grows so no rehash happens while inserting the reserved count
    unordered_map<int, int> r;
    r.reserve(1000);
    size_t bc = r.bucket_count();
    CHECK(float(1000) <= float(bc) * r.max_load_factor() + 1e-6f);
    for (int i = 0; i < 1000; ++i)
        r[i] = i;
    CHECK(r.bucket_count() == bc); // no growth needed
    CHECK(r.size() == 1000);
}

// ---------------------------------------------------------------------------
// erase_if
// ---------------------------------------------------------------------------
void test_erase_if() {
    unordered_map<int, int> m;
    for (int i = 0; i < 100; ++i)
        m[i] = i;
    auto removed = std::erase_if(m, [](const std::pair<const int, int>& kv) {
        return (kv.first % 3) == 0;
    });
    CHECK(removed == 34); // 0,3,...,99 -> 34 values
    CHECK(m.size() == 66);
    for (int i = 0; i < 100; ++i)
        CHECK(m.contains(i) == ((i % 3) != 0));

    unordered_set<int> s;
    for (int i = 0; i < 100; ++i)
        s.insert(i);
    auto r2 = std::erase_if(s, [](int x) { return x >= 50; });
    CHECK(r2 == 50 && s.size() == 50);
}

// ---------------------------------------------------------------------------
// heterogeneous (transparent) lookup
// ---------------------------------------------------------------------------
struct StrHash {
    using is_transparent = void;
    size_t operator()(const string& s) const { return std::hash<string>()(s); }
    size_t operator()(const char* s) const { return std::hash<std::string_view>()(s); }
    size_t operator()(std::string_view s) const { return std::hash<std::string_view>()(s); }
};
struct StrEq {
    using is_transparent = void;
    bool operator()(std::string_view a, std::string_view b) const { return a == b; }
};

void test_transparent_lookup() {
    unordered_map<string, int, StrHash, StrEq> m;
    m["hello"] = 1;
    m["world"] = 2;
    // look up with a string_view / const char* without constructing a string
    std::string_view k = "hello";
    CHECK(m.find(k) != m.end());
    CHECK(m.find(k)->second == 1);
    CHECK(m.count(std::string_view("world")) == 1);
    CHECK(m.contains(std::string_view("nope")) == false);
    auto er = m.equal_range(std::string_view("world"));
    CHECK(er.first != er.second);
}

// ---------------------------------------------------------------------------
// >=10k randomized stress: map mirrored against a plain-array model
// ---------------------------------------------------------------------------
void test_stress_map() {
    const int K = 20000;
    static int model[20000];
    static bool present[20000];
    for (int i = 0; i < K; ++i) {
        model[i] = 0;
        present[i] = false;
    }
    size_t model_size = 0;

    unordered_map<int, int> m;
    Rng rng(0xdeadbeefcafef00dull);

    const int OPS = 120000;
    for (int step = 0; step < OPS; ++step) {
        int key = int(rng.below(K));
        int op = int(rng.below(10));
        if (op < 5) {
            // insert or overwrite
            int val = int(rng.next() & 0x7fffffff);
            m[key] = val;
            model[key] = val;
            if (!present[key]) {
                present[key] = true;
                ++model_size;
            }
        } else if (op < 7) {
            // insert-only (no overwrite)
            int val = int(rng.next() & 0x7fffffff);
            auto r = m.insert(std::pair<int, int>(key, val));
            if (!present[key]) {
                CHECK(r.second);
                present[key] = true;
                model[key] = val;
                ++model_size;
            } else {
                CHECK(!r.second);
            }
        } else if (op < 9) {
            // erase
            size_t er = m.erase(key);
            if (present[key]) {
                CHECK(er == 1);
                present[key] = false;
                --model_size;
            } else {
                CHECK(er == 0);
            }
        } else {
            // find/verify
            auto it = m.find(key);
            if (present[key]) {
                CHECK(it != m.end() && it->second == model[key]);
            } else {
                CHECK(it == m.end());
            }
        }
        CHECK(m.size() == model_size);

        // full equality sweep periodically
        if ((step & 0x3fff) == 0) {
            size_t seen = 0;
            for (auto it = m.begin(); it != m.end(); ++it) {
                CHECK(present[it->first]);
                CHECK(model[it->first] == it->second);
                ++seen;
            }
            CHECK(seen == model_size);
        }
    }

    // final: every model entry matches, iteration count exact
    size_t seen = 0;
    for (auto it = m.begin(); it != m.end(); ++it) {
        CHECK(present[it->first] && model[it->first] == it->second);
        ++seen;
    }
    CHECK(seen == model_size && m.size() == model_size);
    for (int k = 0; k < K; ++k) {
        auto it = m.find(k);
        if (present[k])
            CHECK(it != m.end() && it->second == model[k]);
        else
            CHECK(it == m.end());
    }
}

// ---------------------------------------------------------------------------
// >=10k randomized stress: set mirrored against a plain-array model
// ---------------------------------------------------------------------------
void test_stress_set() {
    const int K = 15000;
    static bool present[15000];
    for (int i = 0; i < K; ++i)
        present[i] = false;
    size_t model_size = 0;

    unordered_set<int> s;
    Rng rng(0x123456789abcdefull);

    const int OPS = 100000;
    for (int step = 0; step < OPS; ++step) {
        int key = int(rng.below(K));
        int op = int(rng.below(4));
        if (op < 2) {
            auto r = s.insert(key);
            if (!present[key]) {
                CHECK(r.second);
                present[key] = true;
                ++model_size;
            } else {
                CHECK(!r.second);
            }
        } else if (op == 2) {
            size_t er = s.erase(key);
            if (present[key]) {
                CHECK(er == 1);
                present[key] = false;
                --model_size;
            } else {
                CHECK(er == 0);
            }
        } else {
            CHECK(s.contains(key) == present[key]);
        }
        CHECK(s.size() == model_size);
        if ((step & 0x3fff) == 0) {
            size_t seen = 0;
            for (auto it = s.begin(); it != s.end(); ++it) {
                CHECK(present[*it]);
                ++seen;
            }
            CHECK(seen == model_size);
        }
    }
    for (int k = 0; k < K; ++k)
        CHECK(s.contains(k) == present[k]);
}

// Regression: hash_pow2_ceil must be defined for EVERY input. For n > 2^63 it
// used to evaluate size_t(1) << 64 (shift-count >= width -> UB); it now clamps
// to the highest representable power of two (2^63). Reachable through
// rehash()/reserve(); we do NOT actually call rehash(SIZE_MAX) (it would request
// 2^63 * sizeof(node*) bytes and trap on the allocation) — the helper's boundary
// semantics are pinned directly instead.
void test_hash_pow2_ceil_boundaries() {
    using std::detail::hash_pow2_ceil;
    // Width-portable: the highest representable power of two is 2^(digits-1)
    // (2^63 on LP64, 2^31 on ILP32); the second-highest is 2^(digits-2). Spelling
    // these with the literal 63/62 would be shift-count-UB on a 32-bit size_t.
    constexpr int digits = std::numeric_limits<size_t>::digits;
    constexpr size_t top  = size_t(1) << (digits - 1);   // 2^63 / 2^31
    constexpr size_t top2 = size_t(1) << (digits - 2);   // 2^62 / 2^30
    // Compile-time: the huge-constant path is defined and clamps to the top pow2.
    static_assert(hash_pow2_ceil(SIZE_MAX) == top);
    static_assert(hash_pow2_ceil(top) == top);
    static_assert(hash_pow2_ceil(top + 1) == top);
    // Runtime boundary values.
    CHECK(hash_pow2_ceil(0) == 2);
    CHECK(hash_pow2_ceil(1) == 2);
    CHECK(hash_pow2_ceil(2) == 2);
    CHECK(hash_pow2_ceil(3) == 4);
    CHECK(hash_pow2_ceil(5) == 8);
    CHECK(hash_pow2_ceil(top2) == top2);
    CHECK(hash_pow2_ceil(top2 + 1) == top);
    CHECK(hash_pow2_ceil(top) == top);
    CHECK(hash_pow2_ceil(SIZE_MAX) == top);   // no bad shift
}

// Regression: unordered_multimap / unordered_multiset (previously absent from
// the lean profile). Equivalent keys must be stored, kept ADJACENT so count()/
// equal_range() see a contiguous run, and that adjacency must survive rehash
// (the adjacency-preserving multi rehash, not the unique kernel).
static void test_multi_containers() {
    std::unordered_multiset<int> s;
    for (int r = 0; r < 40; ++r) { s.insert(7); s.insert(100 + r); s.insert(7); }
    CHECK(s.size() == 120);
    CHECK(s.count(7) == 80);
    for (int r = 0; r < 40; ++r) CHECK(s.count(100 + r) == 1);
    // equal_range spans exactly the 80 sevens, contiguously.
    auto er = s.equal_range(7);
    size_t n = 0;
    for (auto i = er.first; i != er.second; ++i) { CHECK(*i == 7); ++n; }
    CHECK(n == 80);
    s.rehash(2);   CHECK(s.count(7) == 80);   // shrink attempt -> adjacency holds
    s.rehash(512); CHECK(s.count(7) == 80);   // grow          -> adjacency holds
    CHECK(s.erase(7) == 80);
    CHECK(s.count(7) == 0 && s.size() == 40);

    std::unordered_multimap<int, int> m;
    m.insert({1, 10}); m.insert({1, 11}); m.emplace(1, 12); m.insert({2, 20});
    CHECK(m.size() == 4 && m.count(1) == 3 && m.count(2) == 1);
    int sum = 0; auto mr = m.equal_range(1);
    for (auto i = mr.first; i != mr.second; ++i) { CHECK(i->first == 1); sum += i->second; }
    CHECK(sum == 33);
    // copy-assign preserves duplicates (not deduped).
    std::unordered_multimap<int, int> m2; m2.insert({9, 9});
    m2 = m; CHECK(m2.size() == 4 && m2.count(1) == 3 && m2.count(9) == 0);
    CHECK(m2 == m);
}

// Regression: the local (per-bucket) API — bucket(), bucket_size(),
// local_iterator, begin(n)/end(n) — now provided over the intrusive-list run.
static void test_local_iterators() {
    std::unordered_set<int> s;
    for (int i = 0; i < 200; ++i) s.insert(i);
    size_t total = 0;
    for (size_t b = 0; b < s.bucket_count(); ++b) {
        size_t bs = 0;
        for (auto it = s.begin(b); it != s.end(b); ++it) {
            CHECK(s.bucket(*it) == b);   // every element lands in bucket b
            ++bs;
        }
        CHECK(bs == s.bucket_size(b));
        total += bs;
    }
    CHECK(total == s.size());
}

int main() {
    test_multi_containers();
    test_local_iterators();
    test_hash_pow2_ceil_boundaries();
    test_map_basic();
    test_set_basic();
    test_iterator_walk();
    test_rehash_growth();
    test_cached_hash_no_recompute();
    test_reference_stability();
    test_string_keys();
    test_nontrivial_destruction();
    test_move_only_mapped();
    test_move_only_set();
    test_copy_move_swap();
    test_load_factor();
    test_erase_if();
    test_transparent_lookup();
    test_stress_map();
    test_stress_set();
    return 0;
}
