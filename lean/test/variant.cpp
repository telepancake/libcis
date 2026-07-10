// lean <variant> — the table-driven special-member engine (bits/lean_variant.h)
// against base behaviour. No printing; every check traps on failure
// (lean_test.h). No death test: under -fno-exceptions the variant is never
// valueless, so there is no valueless code path to exercise.
//
// The load-bearing checks:
//   * same-index assignment uses the alternative's ASSIGNMENT operator (1
//     copy/move-assign), never destroy+construct — verified with an op-counting
//     alternative;
//   * copy/move/destroy of a mixed variant with a live-counting alternative
//     returns the live count to exactly 0 with the expected op counts;
//   * the strictly-better gate: variant<int,long,double> stays trivially
//     copyable with zero engine code;
//   * the narrow index shrinks sizeof;
//   * a 10k randomized stress mirrored against a reference model tracking the
//     active alternative, its value, and the global live count.
#include <variant>
#include <string>
#include <memory>          // unique_ptr — move-only alternative
#include <utility>
#include "lean_test.h"

using namespace std;

// ===========================================================================
// Strictly-better-or-equal gate + narrow index (compile-time)
// ===========================================================================

// All alternatives trivially copyable + trivially destructible => the variant
// keeps fully trivial special members: zero table, byte-identical to base.
static_assert(is_trivially_copyable_v<variant<int, long, double>>,
              "all-trivial variant must stay trivially copyable");
static_assert(is_trivially_copyable_v<variant<char>>, "");
static_assert(is_trivially_destructible_v<variant<int, long, double>>, "");

// A variant with a non-trivial alternative is NOT trivially copyable (engine
// path is taken).
static_assert(!is_trivially_copyable_v<variant<int, string>>, "");

// Narrow index: base always spent `unsigned int` (4 B) on the discriminant;
// lean uses the smallest unsigned type that holds the alternative count. These
// hold on both LP64 and ILP32 (char/short widths are arch-independent).
static_assert(sizeof(variant<char>) == 2, "1 alt: char storage + uchar index");
static_assert(sizeof(variant<char, short>) == 4, "2 alts: short storage + uchar index");
static_assert(sizeof(variant<char, char, char>) == 2, "uchar index, char storage");
// Pointer-width-relative: storage is one pointer, index one byte after it.
static_assert(sizeof(variant<void*, char>) == 2 * sizeof(void*),
              "pointer storage + uchar index rounds to 2 words");

// ===========================================================================
// Op-counting alternative
// ===========================================================================

namespace {

struct Counters {
    long ctor = 0, copy_ctor = 0, move_ctor = 0;
    long copy_assign = 0, move_assign = 0, dtor = 0;
    void reset() { ctor = copy_ctor = move_ctor = copy_assign = move_assign = dtor = 0; }
    long live() const { return ctor + copy_ctor + move_ctor - dtor; }
};
Counters C;

struct Op {
    int v;
    Op(int x = 0) : v(x) { ++C.ctor; }
    Op(const Op& o) : v(o.v) { ++C.copy_ctor; }
    Op(Op&& o) noexcept : v(o.v) { ++C.move_ctor; }
    Op& operator=(const Op& o) { v = o.v; ++C.copy_assign; return *this; }
    Op& operator=(Op&& o) noexcept { v = o.v; ++C.move_assign; return *this; }
    ~Op() { ++C.dtor; }
    bool operator==(const Op& o) const { return v == o.v; }
    auto operator<=>(const Op& o) const { return v <=> o.v; }
};

// A live-counting alternative whose move PRESERVES the source value (so the
// reference model can treat move like copy). Non-trivial => engine path.
long g_live = 0;
struct Counted {
    long v;
    Counted(long x = 0) : v(x) { ++g_live; }
    Counted(const Counted& o) : v(o.v) { ++g_live; }
    Counted(Counted&& o) noexcept : v(o.v) { ++g_live; }
    Counted& operator=(const Counted& o) = default;   // does not change live
    Counted& operator=(Counted&& o) noexcept = default;
    ~Counted() { --g_live; }
    bool operator==(const Counted& o) const { return v == o.v; }
    auto operator<=>(const Counted& o) const { return v <=> o.v; }
};

struct Rng {
    unsigned long s;
    explicit Rng(unsigned long seed) : s(seed) {}
    unsigned long next() { s ^= s << 13; s ^= s >> 7; s ^= s << 17; return s; }
    size_t bounded(size_t n) { return n ? next() % n : 0; }
};

} // namespace

// ===========================================================================
// Construction / observers / get / get_if / holds_alternative
// ===========================================================================

void test_construct_and_get() {
    // default constructs alternative 0
    variant<int, string> d;
    CHECK(d.index() == 0);
    CHECK(get<0>(d) == 0);
    CHECK(!d.valueless_by_exception());

    // converting constructor picks the best match
    variant<long, string, double> a = 7L;
    CHECK(a.index() == 0 && get<long>(a) == 7);
    variant<long, string, double> b = string("hi");
    CHECK(b.index() == 1 && get<string>(b) == "hi");
    variant<long, string, double> c = 2.5;
    CHECK(c.index() == 2 && get<double>(c) == 2.5);

    // in_place_index / in_place_type
    variant<long, string, double> e(in_place_index<1>, 3u, 'z');
    CHECK(e.index() == 1 && get<1>(e) == "zzz");
    variant<long, string, double> f(in_place_type<double>, 9.0);
    CHECK(f.index() == 2 && get<2>(f) == 9.0);

    // holds_alternative
    CHECK(holds_alternative<string>(b));
    CHECK(!holds_alternative<long>(b));

    // get_if
    CHECK(get_if<1>(&b) != nullptr && *get_if<1>(&b) == "hi");
    CHECK(get_if<0>(&b) == nullptr);
    CHECK(get_if<string>(&b) != nullptr);
    CHECK(get_if<long>(&b) == nullptr);
    const variant<long, string, double>& cb = b;
    CHECK(get_if<string>(&cb) != nullptr && *get_if<string>(&cb) == "hi");

    // valueless is unreachable in this profile
    CHECK(!a.valueless_by_exception() && a.index() != variant_npos);
}

// ===========================================================================
// same-index assignment uses the ASSIGNMENT operator (op-count witness)
// ===========================================================================

void test_same_index_assignment_semantics() {
    // ---- copy assign, SAME index -> exactly 1 copy_assign, no ctor/dtor ----
    {
        variant<Op, long> a(in_place_type<Op>, 1);
        variant<Op, long> b(in_place_type<Op>, 2);
        C.reset();
        a = b;                                   // same index (0): value assign
        CHECK(C.copy_assign == 1);
        CHECK(C.copy_ctor == 0 && C.move_ctor == 0 && C.dtor == 0);
        CHECK(get<Op>(a).v == 2);
    }
    // ---- move assign, SAME index -> exactly 1 move_assign -----------------
    {
        variant<Op, long> a(in_place_type<Op>, 1);
        variant<Op, long> b(in_place_type<Op>, 5);
        C.reset();
        a = std::move(b);
        CHECK(C.move_assign == 1);
        CHECK(C.copy_ctor == 0 && C.move_ctor == 0 && C.dtor == 0);
        CHECK(get<Op>(a).v == 5);
    }
    // ---- copy assign, DIFFERENT index -> destroy old + construct new ------
    {
        variant<Op, long> a(in_place_type<Op>, 1);   // index 0
        variant<Op, long> b(in_place_index<1>, 42L); // index 1 (trivial)
        C.reset();
        a = b;                                        // 0 -> 1: destroy Op
        CHECK(C.dtor == 1);                           // old Op destroyed
        CHECK(C.copy_assign == 0);                    // NOT value-assigned
        CHECK(a.index() == 1 && get<1>(a) == 42);
    }
    // ---- move assign, DIFFERENT index: 0(long) -> 1(Op), construct Op -----
    {
        variant<long, Op> a(in_place_index<0>, 1L);   // index 0 (trivial)
        variant<long, Op> b(in_place_type<Op>, 9);    // index 1
        C.reset();
        a = std::move(b);                             // 0 -> 1: move-construct Op
        CHECK(C.move_ctor == 1);
        CHECK(C.move_assign == 0 && C.copy_ctor == 0);
        CHECK(a.index() == 1 && get<Op>(a).v == 9);
    }
}

// ===========================================================================
// copy / move / destroy of a mixed variant: live count returns to 0
// ===========================================================================

void test_live_counts() {
    g_live = 0;
    {
        variant<long, Counted, double> a(in_place_type<Counted>, 11);
        CHECK(g_live == 1);
        variant<long, Counted, double> b = a;         // copy ctor
        CHECK(g_live == 2 && get<Counted>(b).v == 11);
        variant<long, Counted, double> c = std::move(a); // move ctor (preserves src)
        CHECK(get<Counted>(c).v == 11);
        // a still holds a (moved-from but live) Counted
        CHECK(a.index() == 1);
        variant<long, Counted, double> d(3.0);        // double, no Counted
        d = b;                                         // cross(2->1): construct Counted
        CHECK(get<Counted>(d).v == 11);
        d = 5L;                                        // cross(1->0): destroy Counted
        CHECK(d.index() == 0);
        b = c;                                         // same index: value assign
        CHECK(get<Counted>(b).v == 11);
    }
    CHECK(g_live == 0);                                // every Counted destroyed
}

// ===========================================================================
// emplace
// ===========================================================================

void test_emplace() {
    g_live = 0;
    variant<long, Counted, string> v(4L);
    Counted& r = v.emplace<Counted>(21);
    CHECK(v.index() == 1 && r.v == 21 && g_live == 1);
    string& s = v.emplace<2>(3u, 'k');                 // emplace<I>, destroys Counted
    CHECK(v.index() == 2 && s == "kkk" && g_live == 0);
    v.emplace<Counted>(99);
    CHECK(v.index() == 1 && get<Counted>(v).v == 99 && g_live == 1);
    v.emplace<0>(7L);
    CHECK(v.index() == 0 && get<0>(v) == 7 && g_live == 0);
    // emplace with initializer_list
    variant<long, string> w(0L);
    w.emplace<1>({'a', 'b', 'c'});
    CHECK(get<1>(w) == "abc");
}

// ===========================================================================
// swap
// ===========================================================================

void test_swap() {
    g_live = 0;
    // same-index swap
    {
        variant<long, Counted, string> a(in_place_type<Counted>, 1);
        variant<long, Counted, string> b(in_place_type<Counted>, 2);
        swap(a, b);
        CHECK(get<Counted>(a).v == 2 && get<Counted>(b).v == 1);
        a.swap(b);
        CHECK(get<Counted>(a).v == 1 && get<Counted>(b).v == 2);
    }
    // cross-index swap
    {
        variant<long, Counted, string> a(in_place_type<Counted>, 7);
        variant<long, Counted, string> b(string("payload"));
        swap(a, b);
        CHECK(a.index() == 2 && get<2>(a) == "payload");
        CHECK(b.index() == 1 && get<Counted>(b).v == 7);
    }
    CHECK(g_live == 0);
}

// ===========================================================================
// visit — return values, and multi-variant visitation
// ===========================================================================

void test_visit() {
    variant<long, string, double> v = string("abcd");

    // typed return
    long n = visit([](const auto& x) -> long {
        if constexpr (is_same_v<decay_t<decltype(x)>, string>) return (long)x.size();
        else return (long)x;
    }, v);
    CHECK(n == 4);

    v = 10L;
    n = visit([](const auto& x) -> long {
        if constexpr (is_same_v<decay_t<decltype(x)>, string>) return (long)x.size();
        else return (long)x;
    }, v);
    CHECK(n == 10);

    // visit<R> explicit return type
    double d = visit<double>([](auto x) -> double { return (double)x + 1; },
                             variant<int, double>(2));
    CHECK(d == 3.0);

    // multi-variant visit (base supports it)
    variant<int, char> a = 3;
    variant<int, char> b = 'A';
    int sum = visit([](auto x, auto y) -> int { return (int)x + (int)y; }, a, b);
    CHECK(sum == 3 + 65);

    // mutating visit
    variant<long, string> m = string("x");
    visit([](auto& x) {
        if constexpr (is_same_v<decay_t<decltype(x)>, string>) x += "yz";
    }, m);
    CHECK(get<string>(m) == "xyz");
}

// ===========================================================================
// comparisons
// ===========================================================================

void test_comparisons() {
    using V = variant<int, string>;
    V a(1), b(1), c(2), s(string("z"));

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b && a >= b);
    // different index compares by index first
    CHECK(a < s);          // index 0 < index 1
    CHECK(s > c);

    // three-way
    CHECK((a <=> b) == 0);
    CHECK((a <=> c) < 0);
    CHECK((s <=> a) > 0);

    // string alternative value comparison at same index
    V p(string("aaa")), q(string("aab"));
    CHECK(p < q && (p <=> q) < 0);
}

// ===========================================================================
// trivial variant round-trips (gate path: no engine)
// ===========================================================================

void test_trivial_roundtrip() {
    variant<int, long, double> a = 3;
    variant<int, long, double> b = a;             // trivial copy
    CHECK(b.index() == 0 && get<0>(b) == 3);
    a = 4.5;
    CHECK(a.index() == 2 && get<2>(a) == 4.5);
    b = a;                                        // trivial (defaulted) assign
    CHECK(b.index() == 2 && get<2>(b) == 4.5);
    variant<int, long, double> c = std::move(a);
    CHECK(c.index() == 2 && get<2>(c) == 4.5);
}

// ===========================================================================
// move-only alternative (copy ops deleted; guarded null thunks)
// ===========================================================================

void test_move_only() {
    variant<unique_ptr<int>, int> v(in_place_index<0>, new int(5));
    CHECK(v.index() == 0 && *get<0>(v) == 5);
    variant<unique_ptr<int>, int> m = std::move(v);   // move ctor
    CHECK(*get<0>(m) == 5);
    variant<unique_ptr<int>, int> w(3);
    w = std::move(m);                                 // move assign cross
    CHECK(w.index() == 0 && *get<0>(w) == 5);
    w.emplace<0>(new int(8));
    CHECK(*get<0>(w) == 8);
    static_assert(!is_copy_constructible_v<variant<unique_ptr<int>, int>>,
                  "move-only alternative deletes variant copy");
}

// ===========================================================================
// 10k randomized stress vs a reference model (index + value + live count)
// ===========================================================================

void test_stress() {
    constexpr size_t N = 64;
    using V = variant<long, Counted, double>;

    struct Ref { int idx; long lv; double dv; };  // mirror of active alt + value
    static Ref model[N];

    g_live = 0;
    // storage for the variants (default-constructed at index 0 = long 0)
    static char buf[N * sizeof(V)];
    V* vs = reinterpret_cast<V*>(buf);
    for (size_t i = 0; i < N; ++i) {
        ::new (&vs[i]) V(0L);
        model[i] = {0, 0, 0};
    }

    auto count_counted = [&]() {
        long c = 0;
        for (size_t i = 0; i < N; ++i) if (model[i].idx == 1) ++c;
        return c;
    };
    auto check_one = [&](size_t i) {
        CHECK(vs[i].index() == (size_t)model[i].idx);
        CHECK(!vs[i].valueless_by_exception());
        if (model[i].idx == 0) CHECK(get<0>(vs[i]) == model[i].lv);
        else if (model[i].idx == 1) CHECK(get<Counted>(vs[i]).v == model[i].lv);
        else CHECK(get<2>(vs[i]) == model[i].dv);
    };

    Rng rng(0x1234567890abcdefULL);
    for (int iter = 0; iter < 12000; ++iter) {
        size_t i = rng.bounded(N);
        int op = (int)rng.bounded(7);
        switch (op) {
        case 0: {                                  // assign long
            long x = (long)(rng.next() & 0xffff);
            vs[i] = x;
            model[i] = {0, x, 0};
            break;
        }
        case 1: {                                  // assign Counted (converting)
            long x = (long)(rng.next() & 0xffff);
            vs[i] = Counted(x);
            model[i] = {1, x, 0};
            break;
        }
        case 2: {                                  // assign double
            double x = (double)(rng.next() & 0xffff) + 0.25;
            vs[i] = x;
            model[i] = {2, 0, x};
            break;
        }
        case 3: {                                  // emplace Counted
            long x = (long)(rng.next() & 0xffff);
            vs[i].emplace<Counted>(x);
            model[i] = {1, x, 0};
            break;
        }
        case 4: {                                  // copy from another
            size_t j = rng.bounded(N);
            vs[i] = vs[j];
            model[i] = model[j];
            break;
        }
        case 5: {                                  // move from another (value-preserving)
            size_t j = rng.bounded(N);
            vs[i] = std::move(vs[j]);
            model[i] = model[j];                   // Counted move preserves source value
            break;
        }
        case 6: {                                  // swap two
            size_t j = rng.bounded(N);
            swap(vs[i], vs[j]);
            Ref t = model[i]; model[i] = model[j]; model[j] = t;
            break;
        }
        }
        check_one(i);
        // global live count must always equal the number of Counted-holding slots
        CHECK(g_live == count_counted());
    }

    // final full sweep, then tear down: live must return to 0
    for (size_t i = 0; i < N; ++i) check_one(i);
    for (size_t i = 0; i < N; ++i) vs[i].~V();
    CHECK(g_live == 0);
}

int main() {
    test_construct_and_get();
    test_same_index_assignment_semantics();
    test_live_counts();
    test_emplace();
    test_swap();
    test_visit();
    test_comparisons();
    test_trivial_roundtrip();
    test_move_only();
    test_stress();
    return 0;
}
