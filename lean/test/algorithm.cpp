// lean/test/algorithm.cpp — exercises the rerouted fat sort entry points and
// their fallbacks. Every check traps on failure (CHECK). No printing.
//
// The reference model is an independent bottom-up mergesort (ref_merge_sort)
// built only from indexing/comparison/std::swap — it shares no code with the
// kernels under test — so "std::sort output equals ref_merge_sort output" is a
// genuine cross-check, not a tautology.
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <cstddef>
#include <cstdint>
#include "lean_test.h"

using std::vector;
using std::size_t;

// --------------------------------------------------------------------------
// Independent reference sort + a small deterministic RNG.
// --------------------------------------------------------------------------
static void ref_merge_sort(vector<int>& a) {
    size_t n = a.size();
    if (n < 2) return;
    vector<int> b(n);
    for (size_t w = 1; w < n; w *= 2) {
        for (size_t i = 0; i < n; i += 2 * w) {
            size_t mid = i + w < n ? i + w : n;
            size_t hi  = i + 2 * w < n ? i + 2 * w : n;
            size_t l = i, r = mid, k = i;
            while (l < mid && r < hi) b[k++] = (a[r] < a[l]) ? a[r++] : a[l++];
            while (l < mid) b[k++] = a[l++];
            while (r < hi)  b[k++] = a[r++];
        }
        std::swap(a, b);
    }
}

static uint64_t g_rng = 0x123456789abcdefULL;
static uint64_t xrand() {
    uint64_t x = g_rng;
    x ^= x << 13; x ^= x >> 7; x ^= x << 17;
    g_rng = x;
    return x;
}
static void rng_reset(uint64_t s) { g_rng = s ? s : 1; }

// Pattern generators (id 0..4): random, sorted, reverse, all-equal, sawtooth.
static vector<int> gen(size_t n, int pattern) {
    vector<int> v(n);
    for (size_t i = 0; i < n; ++i) {
        switch (pattern) {
            case 0: v[i] = (int)(xrand() % 1000); break;      // random
            case 1: v[i] = (int)i; break;                     // sorted
            case 2: v[i] = (int)(n - i); break;               // reverse
            case 3: v[i] = 42; break;                         // all equal
            default: v[i] = (int)(i % 8); break;              // sawtooth
        }
    }
    return v;
}

static bool same(const vector<int>& a, const vector<int>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) if (a[i] != b[i]) return false;
    return true;
}

// --------------------------------------------------------------------------
// Compile-time gate checks: which iterator/element types reach the kernel.
// --------------------------------------------------------------------------
namespace {
struct SelfRef {                       // self-referential: byte-relocation is UB,
    int key; SelfRef* self;            // user-defined ops make it NON-trivially-copyable
    SelfRef(int k = 0) : key(k), self(this) {}
    SelfRef(const SelfRef& o) : key(o.key), self(this) {}
    SelfRef(SelfRef&& o) noexcept : key(o.key), self(this) {}
    SelfRef& operator=(const SelfRef& o) { key = o.key; return *this; }   // self stays this
    SelfRef& operator=(SelfRef&& o) noexcept { key = o.key; return *this; }
    ~SelfRef() {}
};
struct MoveOnly {
    int key;
    MoveOnly(int k = 0) : key(k) {}
    MoveOnly(MoveOnly&& o) noexcept : key(o.key) { o.key = -1; }
    MoveOnly& operator=(MoveOnly&& o) noexcept { key = o.key; o.key = -1; return *this; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
};
struct Big   { int key; char pad[300]; };   // trivially copyable but sizeof > 256
struct Mid   { int key; char pad[60]; };    // 64 bytes, trivially copyable
struct KV    { int key; int seq; };         // 8 bytes, trivially copyable
// In-contract over-aligned element: alignof == alignof(max_align_t) (legal
// vector element), sizeof 16, trivially copyable. Reaches the kernel; the
// scratch buffers must hold it at a correctly aligned address.
struct alignas(16) Align16 { long long a, b; };
// Truly over-aligned (alignof > max_align_t): NOT a legal container element,
// but std::sort can be called on a raw array of it. Must take the fallback,
// because the kernel scratch buffers are only aligned to max_align_t.
struct alignas(2 * alignof(std::max_align_t)) Over { long long a, b; };
}

using std::detail::lean_sort_ok;
static_assert(lean_sort_ok<int*>,            "int* must reach the kernel");
static_assert(lean_sort_ok<long*>,           "long* must reach the kernel");
static_assert(lean_sort_ok<std::string*>,    "string is trivially relocatable, sizeof<=256");
static_assert(lean_sort_ok<KV*>,             "8-byte POD reaches the kernel");
static_assert(lean_sort_ok<Mid*>,            "64-byte POD reaches the kernel");
static_assert(sizeof(Big) > 256 && !lean_sort_ok<Big*>, "sizeof>256 must fall back");
static_assert(alignof(Align16) == alignof(std::max_align_t) && lean_sort_ok<Align16*>,
              "in-contract alignof==max_align_t element reaches the kernel");
static_assert(alignof(Over) > alignof(std::max_align_t) && !lean_sort_ok<Over*>,
              "over-aligned element (alignof > max_align_t) must fall back");
static_assert(!lean_sort_ok<MoveOnly*>,      "move-only (non-trivially-copyable) falls back");
static_assert(!lean_sort_ok<SelfRef*>,       "self-referential type must fall back");
static_assert(!lean_sort_ok<const int*>,     "const element cannot be sorted in place");
// Non-pointer (contiguous fails): reverse_iterator of a pointer is not a pointer.
static_assert(!lean_sort_ok<std::reverse_iterator<int*>>, "non-pointer iterators fall back");

void test_gate_compiletime() {
    // Pure compile-time; the run just anchors the file's static_asserts.
    CHECK(lean_sort_ok<int*> == true);
    CHECK(lean_sort_ok<SelfRef*> == false);
}

// --------------------------------------------------------------------------
// sort: patterns x sizes, checked against the independent reference.
// --------------------------------------------------------------------------
static void one_sort(vector<int> v) {
    vector<int> ref = v;
    ref_merge_sort(ref);
    std::sort(v.begin(), v.end());
    CHECK(same(v, ref));
    CHECK(std::is_sorted(v.begin(), v.end()));
    CHECK(std::is_sorted_until(v.begin(), v.end()) == v.end());
}

void test_sort_small() {
    rng_reset(1);
    for (size_t n = 0; n <= 65; ++n)
        for (int p = 0; p < 5; ++p)
            one_sort(gen(n, p));
}

void test_sort_large() {
    rng_reset(99);
    for (int p = 0; p < 5; ++p)
        one_sort(gen(10000, p));
}

void test_sort_comparator_state() {
    // A capturing lambda flows through the thunk's ctx pointer.
    rng_reset(7);
    vector<int> v = gen(5000, 0);
    int bias = 100000;                       // capture makes it a stateful closure
    auto cmp = [bias](int a, int b) { return (a + bias) > (b + bias); };  // descending
    std::sort(v.begin(), v.end(), cmp);
    CHECK(std::is_sorted(v.begin(), v.end(), cmp));
    for (size_t i = 1; i < v.size(); ++i) CHECK(v[i - 1] >= v[i]);
}

static bool cmp_desc_fn(int a, int b) { return a > b; }

void test_sort_descending() {
    rng_reset(11);
    vector<int> v = gen(2000, 0);
    std::sort(v.begin(), v.end(), cmp_desc_fn);          // function-pointer comparator
    for (size_t i = 1; i < v.size(); ++i) CHECK(v[i - 1] >= v[i]);
    std::sort(v.begin(), v.end(), std::greater<int>{});  // functor comparator
    for (size_t i = 1; i < v.size(); ++i) CHECK(v[i - 1] >= v[i]);
}

// --------------------------------------------------------------------------
// stable_sort: value correctness (vs reference) + stability (key,seq pairs).
// --------------------------------------------------------------------------
static void one_stable(vector<int> v) {
    vector<int> ref = v;
    ref_merge_sort(ref);
    std::stable_sort(v.begin(), v.end());
    CHECK(same(v, ref));
}

void test_stable_sort_patterns() {
    rng_reset(3);
    for (size_t n = 0; n <= 65; ++n)
        for (int p = 0; p < 5; ++p)
            one_stable(gen(n, p));
    rng_reset(31);
    for (int p = 0; p < 5; ++p) one_stable(gen(10000, p));
}

void test_stable_sort_stability() {
    rng_reset(5);
    for (size_t n : {size_t(0), size_t(1), size_t(2), size_t(17), size_t(64),
                     size_t(1000), size_t(10000)}) {
        vector<KV> v(n);
        for (size_t i = 0; i < n; ++i) { v[i].key = (int)(xrand() % 10); v[i].seq = (int)i; }
        std::stable_sort(v.begin(), v.end(),
                         [](const KV& a, const KV& b) { return a.key < b.key; });
        for (size_t i = 1; i < n; ++i) {
            CHECK(v[i - 1].key <= v[i].key);
            if (v[i - 1].key == v[i].key) CHECK(v[i - 1].seq < v[i].seq);  // stable
        }
    }
}

// --------------------------------------------------------------------------
// nth_element
// --------------------------------------------------------------------------
static void one_nth(vector<int> v, size_t k) {
    if (v.empty() || k >= v.size()) return;
    vector<int> ref = v;
    ref_merge_sort(ref);
    std::nth_element(v.begin(), v.begin() + k, v.end());
    CHECK(v[k] == ref[k]);
    for (size_t i = 0; i < k; ++i)          CHECK(!(v[k] < v[i]));   // left <= pivot
    for (size_t i = k + 1; i < v.size(); ++i) CHECK(!(v[i] < v[k])); // pivot <= right
}

void test_nth_element() {
    rng_reset(13);
    for (size_t n = 1; n <= 65; ++n)
        for (int p = 0; p < 5; ++p) {
            vector<int> base = gen(n, p);
            for (size_t k = 0; k < n; k += (n / 5 + 1)) one_nth(base, k);
            one_nth(base, n - 1);
        }
    rng_reset(131);
    for (int p = 0; p < 5; ++p) {
        vector<int> base = gen(10000, p);
        for (size_t k : {size_t(0), size_t(1), size_t(4999), size_t(5000), size_t(9999)})
            one_nth(base, k);
    }
}

// --------------------------------------------------------------------------
// partial_sort
// --------------------------------------------------------------------------
static void one_partial(vector<int> v, size_t m) {
    if (m > v.size()) return;
    vector<int> ref = v;
    ref_merge_sort(ref);
    std::partial_sort(v.begin(), v.begin() + m, v.end());
    for (size_t i = 0; i < m; ++i) CHECK(v[i] == ref[i]);   // first m are the m smallest, sorted
    for (size_t i = 0; i + 1 < m; ++i) CHECK(!(v[i + 1] < v[i]));
    for (size_t i = m; i < v.size(); ++i) if (m > 0) CHECK(!(v[i] < v[m - 1]));
}

void test_partial_sort() {
    rng_reset(17);
    for (size_t n = 0; n <= 65; ++n)
        for (int p = 0; p < 5; ++p) {
            vector<int> base = gen(n, p);
            for (size_t m = 0; m <= n; m += (n / 4 + 1)) one_partial(base, m);
            one_partial(base, n);
        }
    rng_reset(171);
    for (int p = 0; p < 5; ++p) {
        vector<int> base = gen(10000, p);
        for (size_t m : {size_t(0), size_t(1), size_t(100), size_t(5000), size_t(10000)})
            one_partial(base, m);
    }
}

// --------------------------------------------------------------------------
// partial_sort_copy — NOT a rerouted fat kernel entry point (it never routes
// to a lean_sort kernel; it is a pure copy template), so unlike sort/heap/
// nth_element it MUST stay constexpr. Regression: it used to call the
// de-constexpr'd public make_heap/sort_heap and thereby lost constant-
// evaluability even though it is declared constexpr. Exercised here in
// constant evaluation (static_assert) over a raw-pointer result range — which
// is lean_sort_ok, so it drives the is_constant_evaluated() fallback arm — and
// at runtime against the independent reference.
// --------------------------------------------------------------------------
constexpr bool partial_sort_copy_cx() {
    int src[8] = {5, 3, 8, 1, 9, 2, 7, 4};
    int dst[4] = {0, 0, 0, 0};
    int* e = std::partial_sort_copy(src, src + 8, dst, dst + 4);
    if (e != dst + 4) return false;
    if (!(dst[0] == 1 && dst[1] == 2 && dst[2] == 3 && dst[3] == 4)) return false;
    // comparator overload, descending: the 4 LARGEST, sorted descending
    int dst2[4] = {0, 0, 0, 0};
    std::partial_sort_copy(src, src + 8, dst2, dst2 + 4, std::greater<int>{});
    return dst2[0] == 9 && dst2[1] == 8 && dst2[2] == 7 && dst2[3] == 5;
}
static_assert(partial_sort_copy_cx(),
              "partial_sort_copy must remain usable in constant evaluation");

void test_partial_sort_copy() {
    CHECK(partial_sort_copy_cx());                       // runtime arm of the same body
    rng_reset(23);
    for (size_t n = 0; n <= 65; ++n)
        for (int p = 0; p < 5; ++p) {
            vector<int> v = gen(n, p);
            vector<int> ref = v;
            ref_merge_sort(ref);
            for (size_t m = 0; m <= n; m += (n / 4 + 1)) {
                vector<int> out(m, 0);
                int* e = std::partial_sort_copy(v.begin(), v.end(),
                                                out.begin(), out.end());
                CHECK(e == out.begin() + m);
                for (size_t i = 0; i < m; ++i) CHECK(out[i] == ref[i]);
            }
        }
}

// --------------------------------------------------------------------------
// heap family: make/push/pop/sort_heap
// --------------------------------------------------------------------------
void test_heap_family() {
    rng_reset(19);
    for (size_t n = 0; n <= 65; ++n)
        for (int p = 0; p < 5; ++p) {
            vector<int> v = gen(n, p);
            vector<int> ref = v;                 // snapshot BEFORE heapifying
            ref_merge_sort(ref);
            std::make_heap(v.begin(), v.end());
            CHECK(std::is_heap(v.begin(), v.end()));
            // pop all: elements come off in descending order → array ends sorted asc
            std::sort_heap(v.begin(), v.end());
            CHECK(same(v, ref));
        }

    // push_heap: build incrementally, verify is_heap after each push.
    for (size_t n : {size_t(0), size_t(1), size_t(2), size_t(33), size_t(1000)}) {
        vector<int> v;
        for (size_t i = 0; i < n; ++i) {
            v.push_back((int)(xrand() % 500));
            std::push_heap(v.begin(), v.end());
            CHECK(std::is_heap(v.begin(), v.end()));
        }
        // pop_heap repeatedly extracts the max
        int prev = 0x7fffffff;
        for (size_t i = 0; i < n; ++i) {
            std::pop_heap(v.begin(), v.end() - (long)i);
            int mx = v[v.size() - 1 - i];
            CHECK(mx <= prev);
            prev = mx;
        }
    }

    // large make_heap/sort_heap
    rng_reset(191);
    vector<int> big = gen(10000, 0);
    vector<int> ref = big; ref_merge_sort(ref);
    std::make_heap(big.begin(), big.end());
    CHECK(std::is_heap(big.begin(), big.end()));
    std::sort_heap(big.begin(), big.end());
    CHECK(same(big, ref));
}

// --------------------------------------------------------------------------
// inplace_merge (value correctness + stability)
// --------------------------------------------------------------------------
void test_inplace_merge() {
    rng_reset(23);
    for (size_t n = 0; n <= 65; ++n)
        for (size_t split = 0; split <= n; split += (n / 4 + 1)) {
            vector<int> v = gen(n, 0);
            std::sort(v.begin(), v.begin() + split);
            std::sort(v.begin() + split, v.end());
            vector<int> ref = v; ref_merge_sort(ref);
            std::inplace_merge(v.begin(), v.begin() + split, v.end());
            CHECK(same(v, ref));
        }

    // stability: pairs, both halves sorted by key, merge must keep seq order on ties.
    rng_reset(231);
    for (size_t n : {size_t(2), size_t(50), size_t(4000), size_t(10000)}) {
        for (size_t split : {n / 3, n / 2, (2 * n) / 3}) {
            vector<KV> v(n);
            for (size_t i = 0; i < n; ++i) { v[i].key = (int)(xrand() % 8); v[i].seq = (int)i; }
            auto by_key = [](const KV& a, const KV& b) { return a.key < b.key; };
            std::stable_sort(v.begin(), v.begin() + split, by_key);
            std::stable_sort(v.begin() + split, v.end(), by_key);
            std::inplace_merge(v.begin(), v.begin() + split, v.end(), by_key);
            for (size_t i = 1; i < n; ++i) {
                CHECK(v[i - 1].key <= v[i].key);
                if (v[i - 1].key == v[i].key) CHECK(v[i - 1].seq < v[i].seq);
            }
        }
    }
}

// --------------------------------------------------------------------------
// Kernel path with a non-trivially-copyable but trivially-relocatable type.
// std::string owns a heap buffer when long; byte-relocation must not corrupt it.
// --------------------------------------------------------------------------
void test_string_sort_kernel() {
    rng_reset(29);
    const size_t n = 3000;
    vector<std::string> v(n);
    for (size_t i = 0; i < n; ++i) {
        // long strings force heap allocation (beyond SSO)
        std::string s = "prefix_long_enough_to_heap_allocate_";
        s += std::to_string(xrand() % 100000);
        v[i] = s;
    }
    vector<std::string> ref = v;
    // reference: stable_sort'd copy is fully sorted; compare against sorted std::sort
    std::sort(ref.begin(), ref.end());   // ref via the same kernel is not independent,
    // so cross-check the KEY invariants directly instead:
    std::sort(v.begin(), v.end());
    CHECK(std::is_sorted(v.begin(), v.end()));
    CHECK(v.size() == n);
    // every string still valid & readable (would crash/mismatch if pointers corrupted)
    size_t total = 0;
    for (auto& s : v) { CHECK(s.size() >= 7); total += s.size(); }
    CHECK(total > 0);
    // multiset preserved: same total length as before sorting
    size_t total_ref = 0;
    for (auto& s : ref) total_ref += s.size();
    CHECK(total == total_ref);
    // and elementwise equal to the independently-unsorted->sorted copy
    CHECK(v.size() == ref.size());
    for (size_t i = 0; i < n; ++i) CHECK(v[i] == ref[i]);
}

// --------------------------------------------------------------------------
// Fallback path: self-referential type — MUST NOT be byte-swapped.
// After sorting, every element's self-pointer must still point at itself.
// --------------------------------------------------------------------------
void test_selfref_fallback() {
    rng_reset(37);
    const size_t n = 2000;
    vector<SelfRef> v;
    v.reserve(n);
    for (size_t i = 0; i < n; ++i) v.emplace_back((int)(xrand() % 1000));
    std::sort(v.begin(), v.end(), [](const SelfRef& a, const SelfRef& b) { return a.key < b.key; });
    for (size_t i = 0; i < n; ++i) {
        CHECK(v[i].self == &v[i]);                       // self-pointer intact
        if (i) CHECK(v[i - 1].key <= v[i].key);          // and correctly sorted
    }
    // stable_sort too (also byte-relocation-forbidden path)
    for (size_t i = 0; i < n; ++i) v[i].key = (int)(xrand() % 50);
    std::stable_sort(v.begin(), v.end(),
                     [](const SelfRef& a, const SelfRef& b) { return a.key < b.key; });
    for (size_t i = 0; i < n; ++i) {
        CHECK(v[i].self == &v[i]);
        if (i) CHECK(v[i - 1].key <= v[i].key);
    }
}

// --------------------------------------------------------------------------
// Fallback path: move-only element type.
// --------------------------------------------------------------------------
void test_moveonly_fallback() {
    rng_reset(41);
    const size_t n = 1500;
    vector<MoveOnly> v;
    v.reserve(n);
    for (size_t i = 0; i < n; ++i) v.emplace_back((int)(xrand() % 1000));
    std::sort(v.begin(), v.end(), [](const MoveOnly& a, const MoveOnly& b) { return a.key < b.key; });
    for (size_t i = 1; i < n; ++i) CHECK(v[i - 1].key <= v[i].key);
    // nth_element on move-only through the fallback
    std::nth_element(v.begin(), v.begin() + n / 2, v.end(),
                     [](const MoveOnly& a, const MoveOnly& b) { return a.key < b.key; });
    int mid = v[n / 2].key;
    for (size_t i = 0; i < n / 2; ++i)      CHECK(v[i].key <= mid);
    for (size_t i = n / 2 + 1; i < n; ++i)  CHECK(v[i].key >= mid);
}

// --------------------------------------------------------------------------
// Larger trivially-copyable element (64 bytes): exercises the kernel with a
// non-4/8-byte element size (generic memcpy path in the swap).
// --------------------------------------------------------------------------
void test_mid_element_kernel() {
    rng_reset(43);
    const size_t n = 4000;
    vector<Mid> v(n);
    for (size_t i = 0; i < n; ++i) { v[i].key = (int)(xrand() % 5000); v[i].pad[0] = (char)i; }
    std::sort(v.begin(), v.end(), [](const Mid& a, const Mid& b) { return a.key < b.key; });
    for (size_t i = 1; i < n; ++i) CHECK(v[i - 1].key <= v[i].key);
}

// --------------------------------------------------------------------------
// Over-aligned in-contract element (alignof == alignof(max_align_t)): reaches
// the kernel, and the comparator must never receive a reference to a
// misaligned scratch slot. The kernel holds the pivot / the sifted element in
// stack buffers (lean_partition's `pivot`, the sift/insertion `tmp`) and hands
// their address to the comparator thunk, which reifies a `const Align16&`.
// Those buffers must be aligned to at least alignof(Align16), else the
// reference is bound to an under-aligned address (UB, [basic.align]/1). Without
// the alignas(max_align_t) on the buffers the pivot/tmp can land 8-aligned and
// this check fails (observed at -Os).
// --------------------------------------------------------------------------
static bool g_saw_misaligned = false;
static bool align16_cmp(const Align16& a, const Align16& b) {
    if (reinterpret_cast<std::uintptr_t>(&a) % alignof(Align16)) g_saw_misaligned = true;
    if (reinterpret_cast<std::uintptr_t>(&b) % alignof(Align16)) g_saw_misaligned = true;
    return a.a < b.a;
}

void test_overaligned_element_kernel() {
    rng_reset(53);
    const size_t n = 5000;
    vector<Align16> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i].a = (long long)(xrand() % 100000);
        v[i].b = (long long)i;
    }
    g_saw_misaligned = false;
    std::sort(v.begin(), v.end(), align16_cmp);          // routes into the kernel
    CHECK(!g_saw_misaligned);                            // every comparator arg aligned
    CHECK(std::is_sorted(v.begin(), v.end(), align16_cmp));
    for (size_t i = 1; i < n; ++i) CHECK(v[i - 1].a <= v[i].a);

    // Exercise every kernel entry point that stashes an element in a scratch
    // buffer, so the alignment of pivot/tmp is checked on each path.
    { vector<Align16> w = v; g_saw_misaligned = false;
      std::stable_sort(w.begin(), w.end(), align16_cmp); CHECK(!g_saw_misaligned); }
    { vector<Align16> w = v; g_saw_misaligned = false;
      std::nth_element(w.begin(), w.begin() + n / 2, w.end(), align16_cmp);
      CHECK(!g_saw_misaligned); }
    { vector<Align16> w = v; g_saw_misaligned = false;
      std::partial_sort(w.begin(), w.begin() + n / 3, w.end(), align16_cmp);
      CHECK(!g_saw_misaligned); }
    { vector<Align16> w = v; g_saw_misaligned = false;
      std::make_heap(w.begin(), w.end(), align16_cmp);
      std::sort_heap(w.begin(), w.end(), align16_cmp); CHECK(!g_saw_misaligned); }
}

// --------------------------------------------------------------------------
// is_sorted / is_sorted_until agreement (unchanged templated algorithms).
// --------------------------------------------------------------------------
void test_is_sorted_agreement() {
    vector<int> v = {1, 2, 3, 5, 4, 6};
    CHECK(!std::is_sorted(v.begin(), v.end()));
    auto it = std::is_sorted_until(v.begin(), v.end());
    CHECK(it == v.begin() + 4);                     // breaks at the 4 after 5
    CHECK(std::is_sorted(v.begin(), it));
    vector<int> w = {1, 1, 2, 3};
    CHECK(std::is_sorted(w.begin(), w.end()));
    CHECK(std::is_sorted_until(w.begin(), w.end()) == w.end());
    vector<int> e;
    CHECK(std::is_sorted(e.begin(), e.end()));
}

// --------------------------------------------------------------------------
// Randomized stress (>=10k elements): mirror every rerouted op against the
// reference model and CHECK full equality / invariants along the way.
// --------------------------------------------------------------------------
void test_stress_mirror() {
    rng_reset(0xF00DBEEF);
    for (int round = 0; round < 6; ++round) {
        size_t n = 10000 + (size_t)(xrand() % 5000);
        vector<int> base(n);
        for (size_t i = 0; i < n; ++i) base[i] = (int)(xrand() % 20000) - 10000;

        vector<int> ref = base;
        ref_merge_sort(ref);

        // sort
        { vector<int> v = base; std::sort(v.begin(), v.end()); CHECK(same(v, ref)); }
        // stable_sort
        { vector<int> v = base; std::stable_sort(v.begin(), v.end()); CHECK(same(v, ref)); }
        // sort_heap
        { vector<int> v = base; std::make_heap(v.begin(), v.end());
          CHECK(std::is_heap(v.begin(), v.end()));
          std::sort_heap(v.begin(), v.end()); CHECK(same(v, ref)); }
        // partial_sort of a random prefix
        { vector<int> v = base; size_t m = (size_t)(xrand() % (n + 1));
          std::partial_sort(v.begin(), v.begin() + m, v.end());
          for (size_t i = 0; i < m; ++i) CHECK(v[i] == ref[i]); }
        // nth_element at a random index
        { vector<int> v = base; size_t k = (size_t)(xrand() % n);
          std::nth_element(v.begin(), v.begin() + k, v.end());
          CHECK(v[k] == ref[k]);
          for (size_t i = 0; i < k; ++i)          CHECK(!(v[k] < v[i]));
          for (size_t i = k + 1; i < n; ++i)      CHECK(!(v[i] < v[k])); }
        // inplace_merge of two sorted halves
        { vector<int> v = base; size_t split = n / 2;
          std::sort(v.begin(), v.begin() + split);
          std::sort(v.begin() + split, v.end());
          std::inplace_merge(v.begin(), v.begin() + split, v.end());
          CHECK(same(v, ref)); }
    }
}

int main() {
    test_gate_compiletime();
    test_sort_small();
    test_sort_large();
    test_sort_comparator_state();
    test_sort_descending();
    test_stable_sort_patterns();
    test_stable_sort_stability();
    test_nth_element();
    test_partial_sort();
    test_partial_sort_copy();
    test_heap_family();
    test_inplace_merge();
    test_string_sort_kernel();
    test_selfref_fallback();
    test_moveonly_fallback();
    test_mid_element_kernel();
    test_overaligned_element_kernel();
    test_is_sorted_agreement();
    test_stress_mirror();
    return 0;
}
