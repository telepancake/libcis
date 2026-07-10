// lean <functional> — the 2-word, no-vtable std::function against the four
// storage modes (one-word inline / fn-pointer / extension-ctor / heap block).
//
// No printing; every check traps on failure (lean_test.h).  A death test
// (fork/waitpid, the pattern from string.cpp / memory.cpp) confirms that calling
// an EMPTY function dies with SIGILL (the __builtin_trap signal) exactly.  The
// heap-freedom matrix uses mallinfo2(): the inline modes (fn ptr, capture-less
// lambda, one-word [ptr] capture, the extension ctor) must move uordblks by
// ZERO across 1000 construct/copy/call/destroy cycles, while the heap modes
// (>=2-word captures, non-trivially-copyable captures) MUST allocate.
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>           // unique_ptr (move-only argument forwarding)
#include <malloc.h>         // mallinfo2 — heap-freedom meter
#include <csignal>          // SIGILL — the __builtin_trap signal
#include <sys/wait.h>       // fork/waitpid death test
#include <unistd.h>
#include "lean_test.h"

using namespace std;

// ---------------------------------------------------------------------------
// sizeof contract — two words, pointer-width-relative (holds on ILP32 too).
// ---------------------------------------------------------------------------
static_assert(sizeof(function<void()>)      == 2 * sizeof(void*), "function is two words");
static_assert(sizeof(function<int(int)>)    == 2 * sizeof(void*), "function is two words");
static_assert(sizeof(function<int&(long,double)>) == 2 * sizeof(void*), "function is two words");

// ===========================================================================
// Shared callables / helpers
// ===========================================================================

static int free_add7(int x) { return x + 7; }
static long free_two(long a, int b) { return a * 100 + b; }

struct Ctx { int v; };
static int ctx_add(Ctx* c, int x) { return c->v + x; }        // extension-ctor target
static long ctx_sub(const Ctx* c, long x) { return c->v - x; } // const-context extension

// A lifetime-counted, copyable callable with a non-trivial dtor -> heap mode.
struct Counted {
    static long live;
    long v;
    explicit Counted(long x) : v(x) { ++live; }
    Counted(const Counted& o) : v(o.v) { ++live; }
    Counted& operator=(const Counted&) = default;
    ~Counted() { --live; }
    int operator()(int x) const { return static_cast<int>(v + x); }
};
long Counted::live = 0;

static size_t heap_now() { return mallinfo2().uordblks; }

// ===========================================================================
// empty / bool / nullptr, and the empty-call death test
// ===========================================================================

void test_empty_and_bool() {
    function<int(int)> e;
    CHECK(!e);
    CHECK(e == nullptr);
    CHECK(nullptr == e);     // C++20 reversed form of operator==(f, nullptr)
    CHECK(!(e != nullptr));  // C++20 synthesized !=

    function<int(int)> n(nullptr);
    CHECK(!n);
    CHECK(n == nullptr);

    function<int(int)> f = free_add7;
    CHECK(static_cast<bool>(f));
    CHECK(f != nullptr);
    CHECK(!(f == nullptr));

    f = nullptr;                 // assign-from-nullptr empties it
    CHECK(!f);
    CHECK(f == nullptr);
}

static void call_empty() {
    function<int(int)> e;
    volatile int z = e(3);       // empty call -> __builtin_trap -> SIGILL
    (void)z;
}

static int death_signal(void (*fn)()) {
    pid_t pid = fork();
    CHECK(pid >= 0);
    if (pid == 0) { fn(); _exit(0); }
    int st = 0;
    CHECK(waitpid(pid, &st, 0) == pid);
    return WIFSIGNALED(st) ? WTERMSIG(st) : -1;
}

void test_empty_call_traps() {
    CHECK(death_signal(call_empty) == SIGILL);
    // A live function under the same harness must NOT die (child exits 0 -> -1).
    CHECK(death_signal([] {
        function<int(int)> f = free_add7;
        CHECK(f(1) == 8);
    }) == -1);
}

// ===========================================================================
// Each storage shape produces the right result
// ===========================================================================

void test_shapes_invoke() {
    // Mode 2: plain function pointer.
    function<int(int)> fp = free_add7;
    CHECK(fp(5) == 12);

    // Mode 2: capture-less lambda (convertible to the exact fn-ptr type).
    function<int(int)> cl = [](int x) { return x * 4; };
    CHECK(cl(5) == 20);

    // Mode 1: one-word [ptr] ("[this]-style") capture.
    int base = 40;
    int* pbase = &base;
    function<int(int)> one = [pbase](int x) { return *pbase + x; };
    CHECK(one(2) == 42);

    // Mode 1: one-word [long] capture (8 bytes == one word on LP64).
    long k = 1000;
    function<long(long)> onek = [k](long x) { return k + x; };
    CHECK(onek(7) == 1007);

    // Mode 3: extension ctor  function(fn, obj) — the user fn is the invoker.
    Ctx c{9};
    function<int(int)> ext(ctx_add, &c);
    CHECK(ext(3) == 12);
    // const-context extension ctor.
    const Ctx cc{50};
    function<long(long)> extc(ctx_sub, &cc);
    CHECK(extc(8) == 42);

    // Mode 4: {obj*, fn*}-style two-word struct with operator() (16 bytes -> heap).
    struct Pair { int* o; int (*f)(int*, int); int operator()(int x) const { return f(o, x); } };
    int obj = 5;
    auto adder = [](int* p, int y) { return *p + y; };
    Pair pr{&obj, +adder};
    function<int(int)> two = pr;
    CHECK(two(6) == 11);

    // Mode 4: 2-word trivially-copyable capture [pa,pb] (16 bytes -> heap).
    long a = 3, b = 2;
    long *pa = &a, *pb = &b;
    function<long()> twoword = [pa, pb]() { return *pa - *pb; };
    CHECK(twoword() == 1);

    // Mode 4: fat capture (4 words -> heap).
    long q1 = 1, q2 = 2, q3 = 3, q4 = 4;
    function<long()> fat = [q1, q2, q3, q4]() { return q1 + q2 + q3 + q4; };
    CHECK(fat() == 10);

    // Mode 4: non-trivially-copyable capture (std::string by value -> heap).
    string s = "abcde";
    function<size_t()> str = [s]() { return s.size(); };
    CHECK(str() == 5);
}

// ===========================================================================
// invoke correctness: void / reference returns, ref args not copied,
// move-only argument forwarded, const invocation, mutation persistence (heap).
// ===========================================================================

void test_invoke_correctness() {
    // void return.
    int sideeffect = 0;
    function<void(int)> vf = [&sideeffect](int x) { sideeffect = x; };  // heap (captures ref)
    vf(99);
    CHECK(sideeffect == 99);

    // reference return — identity of the referent is preserved.
    static int cell = 7;
    function<int&()> rf = []() -> int& { return cell; };
    int& r = rf();
    CHECK(&r == &cell);
    r = 123;
    CHECK(cell == 123);

    // reference argument is NOT copied (address identity through the call).
    struct Big { int data[8]; };
    Big big{};
    const Big* seen = nullptr;
    function<void(const Big&)> af = [&seen](const Big& x) { seen = &x; };
    af(big);
    CHECK(seen == &big);

    // move-only ARGUMENT forwarded through (function target is copyable; arg is not).
    function<int(unique_ptr<int>)> mo = [](unique_ptr<int> p) { return p ? *p : -1; };
    CHECK(mo(unique_ptr<int>(new int(55))) == 55);

    // const invocation: operator() is const and still invokes the target.
    const function<int(int)> cf = free_add7;
    CHECK(cf(1) == 8);

    // Mutable targets persist mutation across calls with STANDARD semantics:
    // the mode-1 gate requires const-invocability, so a mutable lambda — even a
    // one-word one — routes to the heap block and mutates in place.
    function<int()> ctr = [n = 0]() mutable { return ++n; };   // 1 word but mutable -> heap
    CHECK(ctr() == 1);
    CHECK(ctr() == 2);
    CHECK(ctr() == 3);
    function<int()> ctrc = ctr;          // deep copy: independent state from here on
    CHECK(ctrc() == 4);
    CHECK(ctr() == 4);                   // original unaffected by the copy's call
    long pad = 0;
    function<int()> ctr2 = [n = 0, pad]() mutable { (void)pad; return ++n; };
    CHECK(ctr2() == 1);
    CHECK(ctr2() == 2);
    CHECK(ctr2() == 3);
}

// ===========================================================================
// HEAP-FREEDOM matrix (mallinfo2): inline modes never allocate; heap modes do.
// Run early in main, before other noise.  The inline loops call malloc ZERO
// times, so their uordblks delta is exactly 0 independent of arena state.
// ===========================================================================

void test_heap_freedom() {
    // Warm the allocator so any first-touch arena growth is already accounted.
    { string w("warmup"); function<size_t()> f = [w]() { return w.size(); }; volatile size_t z = f(); (void)z; }

    constexpr int N = 1000;
    int obj = 3;
    int* pobj = &obj;
    long k = 5;
    Ctx cx{4};

    // ---- INLINE modes: uordblks delta must be 0 across N full cycles ----
    {
        size_t before = heap_now();
        for (int i = 0; i < N; ++i) {
            function<int(int)> f = free_add7;         // mode 2: fn ptr
            function<int(int)> g = f;                 // copy (memcpy the two words)
            volatile int z = g(i); (void)z;
        }
        CHECK(heap_now() == before);
    }
    {
        size_t before = heap_now();
        for (int i = 0; i < N; ++i) {
            function<int(int)> f = [](int x) { return x + 1; };  // mode 2: capture-less lambda
            function<int(int)> g = f;
            volatile int z = g(i); (void)z;
        }
        CHECK(heap_now() == before);
    }
    {
        size_t before = heap_now();
        for (int i = 0; i < N; ++i) {
            function<int(int)> f = [pobj](int x) { return *pobj + x; };  // mode 1: one-word [ptr]
            function<int(int)> g = f;
            volatile int z = g(i); (void)z;
        }
        CHECK(heap_now() == before);
    }
    {
        size_t before = heap_now();
        for (int i = 0; i < N; ++i) {
            function<long(long)> f = [k](long x) { return k + x; };      // mode 1: one-word [long]
            function<long(long)> g = f;
            volatile long z = g(i); (void)z;
        }
        CHECK(heap_now() == before);
    }
    {
        size_t before = heap_now();
        for (int i = 0; i < N; ++i) {
            function<int(int)> f(ctx_add, &cx);       // mode 3: extension ctor
            function<int(int)> g = f;
            volatile int z = g(i); (void)z;
        }
        CHECK(heap_now() == before);
    }

    // ---- HEAP modes: the meter MUST register allocation ----
    // We assert only that the heap modes allocate (uordblks strictly grows) — the
    // symmetric "returns to baseline" check is deliberately NOT made, because a
    // glibc allocator retains freed small chunks in its per-thread cache and
    // mallinfo2's byte accounting of that is arena-state-dependent (the honest
    // caveat).  Growth on allocation, by contrast, is exact and portable.
    {
        string s = "0123456789";
        vector<function<size_t()>> v;                 // vector itself allocates; measure the
        v.reserve(N);                                 // per-element growth AFTER reserve.
        size_t after_reserve = heap_now();
        for (int i = 0; i < N; ++i)
            v.push_back([s]() { return s.size(); });   // non-trivially-copyable -> heap
        CHECK(heap_now() > after_reserve);            // the N string closures DID allocate
    }
    {
        long k2 = 6;
        long *pa = &k, *pb = &k2;
        vector<function<long()>> v;
        v.reserve(N);
        size_t after_reserve = heap_now();
        for (int i = 0; i < N; ++i)
            v.push_back([pa, pb]() { return *pa + *pb; });  // 2-word capture -> heap
        CHECK(heap_now() > after_reserve);
    }
}

// ===========================================================================
// copy / move / assign / self-assign / swap, incl. heap deep-copy proof
// ===========================================================================

void test_copy_move_swap() {
    // Deep copy of a heap target: mutating the source's captured string after the
    // copy must not affect the copy (proves the payload was cloned, not shared).
    {
        string s = "orig";
        function<string()> src = [s]() { return s; };
        function<string()> cpy = src;
        // rebind src to a different string; cpy must still return "orig".
        string s2 = "changed";
        src = [s2]() { return s2; };
        CHECK(cpy() == "orig");
        CHECK(src() == "changed");
    }

    // self copy-assign and self move-assign are safe.
    {
        string s = "self";
        function<string()> f = [s]() { return s; };
        function<string()>& ref = f;
        ref = f;                       // self copy-assign
        CHECK(f() == "self");
        ref = std::move(f);            // self move-assign
        CHECK(f() == "self");
    }

    // swap across modes: inline <-> heap.
    {
        function<int(int)> inl = free_add7;                    // mode 2
        string s = "xyz";
        function<int(int)> hp = [s](int x) { return (int)s.size() + x; };  // mode 4
        inl.swap(hp);
        CHECK(inl(1) == 4);            // now the heap target
        CHECK(hp(1) == 8);            // now the fn-ptr target
        swap(inl, hp);               // free swap back
        CHECK(inl(1) == 8);
        CHECK(hp(1) == 4);
    }

    // move leaves the source empty.
    {
        string s = "movesrc";
        function<string()> a = [s]() { return s; };
        function<string()> b = std::move(a);
        CHECK(!a);
        CHECK(b() == "movesrc");
    }
}

// ===========================================================================
// reassignment across every mode direction; dtor accounting via Counted.
// ===========================================================================

void test_cross_mode_reassign() {
    CHECK(Counted::live == 0);
    {
        int base = 10; int* pb = &base;
        string s = "hello";
        Ctx cx{4};

        function<int(int)> f;                            // empty
        f = free_add7;                                   // empty   -> fnptr
        CHECK(f(1) == 8);
        f = [pb](int x) { return *pb + x; };             // fnptr   -> one-word
        CHECK(f(2) == 12);
        f = [s](int x) { return (int)s.size() + x; };    // one-word-> heap
        CHECK(f(0) == 5);
        f = free_add7;                                   // heap    -> fnptr
        CHECK(f(3) == 10);
        f = Counted(100);                                // fnptr   -> heap (Counted)
        CHECK(Counted::live == 1);
        CHECK(f(5) == 105);
        f = [](int x) { return x; };                     // heap    -> fnptr (Counted destroyed)
        CHECK(Counted::live == 0);
        CHECK(f(9) == 9);
        f = function<int(int)>(ctx_add, &cx);            // fnptr   -> extension (mode 3)
        CHECK(f(6) == 10);
        f = nullptr;                                     // extension -> empty
        CHECK(!f);
    }
    CHECK(Counted::live == 0);

    // A heap target's destructor runs exactly once on scope exit.
    {
        function<int(int)> f = Counted(1);
        CHECK(Counted::live == 1);
        function<int(int)> g = f;        // deep copy -> a second live Counted
        CHECK(Counted::live == 2);
        function<int(int)> h = std::move(g);
        CHECK(Counted::live == 2);       // move transfers, does not duplicate
        CHECK(!g);
    }
    CHECK(Counted::live == 0);
}

// ===========================================================================
// containers: vector<function> (relocatability) and function in map values.
// ===========================================================================

void test_containers() {
    CHECK(Counted::live == 0);
    {
        vector<function<int(int)>> v;
        for (int i = 0; i < 50; ++i) {
            if (i % 3 == 0)      v.push_back(free_add7);                 // fnptr
            else if (i % 3 == 1) v.push_back([i](int x) { return i + x; }); // one-word
            else                 v.push_back(Counted(i));               // heap
        }
        CHECK(v.size() == 50);
        // exercise relocation: erase from the middle repeatedly (shifts elements).
        for (int k = 0; k < 10; ++k)
            v.erase(v.begin() + 5);
        CHECK(v.size() == 40);
        // all survivors still callable and correct-ish (no crash, deterministic).
        long acc = 0;
        for (auto& f : v) acc += f(1);
        CHECK(acc != 0);
    }
    CHECK(Counted::live == 0);

    // function stored as map values.
    {
        map<int, function<int(int)>> m;
        m[1] = free_add7;
        m[2] = [](int x) { return x * x; };
        string s = "map";
        m[3] = [s](int x) { return (int)s.size() + x; };
        CHECK(m[1](3) == 10);
        CHECK(m[2](4) == 16);
        CHECK(m[3](7) == 10);
        CHECK(m.size() == 3);
    }
}

// ===========================================================================
// Stress: randomized construct/copy/move/swap/call/reassign/reset across a pool
// of mixed-mode functions, checked against a reference model.  Counted live==0.
// ===========================================================================

namespace {
// A pure model of each pooled function's behaviour, so we can predict results
// through arbitrary copy/move/swap shuffling.
struct Model { int kind; long p; bool empty; };
// kind: 0 empty, 1 fnptr, 2 capless, 3 one-word[long], 4 heap Counted, 5 heap 2-word.

int model_call(const Model& m, int x) {
    switch (m.kind) {
        case 1: return x + 7;                 // free_add7
        case 2: return x * 3;
        case 3: return (int)(m.p + x);
        case 4: return (int)(m.p + x);        // Counted(p): operator()(x) == p + x
        case 5: return (int)(m.p - (m.p / 2) + x);
        default: return 0;
    }
}

function<int(int)> make_fn(int kind, long p) {
    switch (kind) {
        case 1: return function<int(int)>(free_add7);
        case 2: return function<int(int)>([](int x) { return x * 3; });
        case 3: return function<int(int)>([p](int x) { return (int)(p + x); });
        case 4: return function<int(int)>(Counted(p));
        case 5: {
            long a = p, b = p / 2;
            return function<int(int)>([a, b](int x) { return (int)(a - b + x); });
        }
        default: return function<int(int)>();
    }
}

// tiny deterministic LCG (no <random> dependency)
struct Rng { uint64_t s; uint32_t next() { s = s * 6364136223846793005ull + 1; return (uint32_t)(s >> 33); } };
} // namespace

void test_stress() {
    CHECK(Counted::live == 0);
    constexpr int POOL = 32;
    function<int(int)> fns[POOL];
    Model mdl[POOL];
    for (int i = 0; i < POOL; ++i) mdl[i] = {0, 0, true};

    Rng rng{0x1234abcdu};
    for (int step = 0; step < 10000; ++step) {
        int op = rng.next() % 6;
        int i = rng.next() % POOL;
        int j = rng.next() % POOL;
        switch (op) {
            case 0: {  // construct/assign a fresh random function
                int kind = 1 + (int)(rng.next() % 5);
                long p = (long)(rng.next() % 97);
                fns[i] = make_fn(kind, p);
                mdl[i] = {kind, p, false};
                break;
            }
            case 1:    // copy i <- j
                fns[i] = fns[j];
                mdl[i] = mdl[j];
                break;
            case 2:    // move i <- j (j becomes empty)
                fns[i] = std::move(fns[j]);
                mdl[i] = mdl[j];
                if (i != j) mdl[j] = {0, 0, true};
                break;
            case 3: {  // swap i, j
                fns[i].swap(fns[j]);
                Model t = mdl[i]; mdl[i] = mdl[j]; mdl[j] = t;
                break;
            }
            case 4:    // call and verify against the model
                if (!mdl[i].empty) {
                    int x = (int)(rng.next() % 1000);
                    CHECK(fns[i](x) == model_call(mdl[i], x));
                }
                break;
            case 5:    // reset to empty
                fns[i] = nullptr;
                mdl[i] = {0, 0, true};
                break;
        }
    }
    // Final sweep: every non-empty slot still matches its model.
    for (int i = 0; i < POOL; ++i) {
        CHECK(static_cast<bool>(fns[i]) == !mdl[i].empty);
        if (!mdl[i].empty)
            CHECK(fns[i](11) == model_call(mdl[i], 11));
    }
    for (int i = 0; i < POOL; ++i) fns[i] = nullptr;
    CHECK(Counted::live == 0);
}

// ===========================================================================

int main() {
    test_heap_freedom();          // first, before any allocator noise
    test_empty_and_bool();
    test_empty_call_traps();
    test_shapes_invoke();
    test_invoke_correctness();
    test_copy_move_swap();
    test_cross_mode_reassign();
    test_containers();
    test_stress();
    return 0;
}
