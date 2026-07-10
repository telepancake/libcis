// lean <memory> — the frugal shared_ptr / weak_ptr family against the
// one-pointer handle + elem-first control block.
//
// No printing; every check traps on failure (lean_test.h).  Death tests
// (fork/waitpid, the pattern from string.cpp) confirm that the documented
// TRAP paths — the multiple-inheritance pointer-adjustment deviation and
// shared_from_this on an unowned object — die with SIGILL exactly, never
// SIGSEGV.  Threads are raw pthreads: <thread> drags in <chrono>/tzdb which the
// lean vector overlay rejects, and the link already carries -lpthread.
#include <memory>
#include <vector>
#include <bits/lean_sp.h>   // detail::sp_cb / sp_payload_offset — single-alloc heuristic
#include <cstdint>
#include <cstddef>
#include <csignal>          // SIGILL — the __builtin_trap signal
#include <sys/wait.h>       // fork/waitpid death tests
#include <unistd.h>
#include <pthread.h>
#include "lean_test.h"

using namespace std;

// ---------------------------------------------------------------------------
// sizeof contract — one pointer, expressed via sizeof(void*) (a 32-bit port
// follows this work, so no hardcoded 8).
// ---------------------------------------------------------------------------
static_assert(sizeof(shared_ptr<int>) == sizeof(void*), "shared_ptr is one pointer");
static_assert(sizeof(weak_ptr<int>)   == sizeof(void*), "weak_ptr is one pointer");
// enable_shared_from_this embeds one weak_ptr, so it is also one pointer.
namespace { struct EsftProbe : enable_shared_from_this<EsftProbe> {}; }
static_assert(sizeof(EsftProbe) == sizeof(void*), "enable_shared_from_this is one pointer");

// ---------------------------------------------------------------------------
// A lifetime-counted payload: the static `live` count returns to 0 iff every
// managed object was destroyed exactly once.
// ---------------------------------------------------------------------------
struct Counted {
    static long live;
    int v;
    explicit Counted(int x = 0) : v(x) { __atomic_add_fetch(&live, 1, __ATOMIC_SEQ_CST); }
    Counted(const Counted& o) : v(o.v) { __atomic_add_fetch(&live, 1, __ATOMIC_SEQ_CST); }
    ~Counted() { __atomic_sub_fetch(&live, 1, __ATOMIC_SEQ_CST); }
};
long Counted::live = 0;

// ---------------------------------------------------------------------------
// Lifecycle + use_count across copy / move / reset / swap / self-assign.
// ---------------------------------------------------------------------------
void test_lifecycle() {
    CHECK(Counted::live == 0);
    {
        shared_ptr<Counted> a = make_shared<Counted>(7);
        CHECK(a.use_count() == 1);
        CHECK(a->v == 7);
        CHECK(Counted::live == 1);

        shared_ptr<Counted> b = a;              // copy
        CHECK(a.use_count() == 2);
        CHECK(b.use_count() == 2);
        CHECK(a.get() == b.get());
        CHECK(Counted::live == 1);              // still one object

        shared_ptr<Counted> c = std::move(b);   // move
        CHECK(!b);
        CHECK(b.use_count() == 0);
        CHECK(c.use_count() == 2);
        CHECK(Counted::live == 1);

        c.reset();                              // reset one holder
        CHECK(!c);
        CHECK(a.use_count() == 1);
        CHECK(Counted::live == 1);

        shared_ptr<Counted> d = make_shared<Counted>(99);
        CHECK(Counted::live == 2);
        a.swap(d);                              // swap
        CHECK(a->v == 99);
        CHECK(d->v == 7);
        CHECK(a.use_count() == 1 && d.use_count() == 1);

        a = a;                                  // self copy-assign (copy-and-swap safe)
        CHECK(a->v == 99);
        CHECK(a.use_count() == 1);
        shared_ptr<Counted>& ar = a;
        a = std::move(ar);                      // self move-assign
        CHECK(a->v == 99);
        CHECK(a.use_count() == 1);
        CHECK(Counted::live == 2);
    }
    CHECK(Counted::live == 0);                  // all destroyed
}

// ---------------------------------------------------------------------------
// weak_ptr lock / expired before and after the strong owners release.
// ---------------------------------------------------------------------------
void test_weak() {
    CHECK(Counted::live == 0);
    weak_ptr<Counted> w;
    CHECK(w.expired());
    CHECK(w.use_count() == 0);
    CHECK(!w.lock());
    {
        shared_ptr<Counted> s = make_shared<Counted>(5);
        w = s;
        CHECK(!w.expired());
        CHECK(w.use_count() == 1);
        shared_ptr<Counted> locked = w.lock();
        CHECK(locked);
        CHECK(locked.get() == s.get());
        CHECK(s.use_count() == 2);              // lock added a strong owner
        CHECK(w.use_count() == 2);
    }
    // All strong owners gone: the object is dead but the block survives while w
    // holds a weak ref.
    CHECK(Counted::live == 0);
    CHECK(w.expired());
    CHECK(w.use_count() == 0);
    CHECK(!w.lock());
    w.reset();
    CHECK(w.expired());
}

// ---------------------------------------------------------------------------
// A reference cycle broken by a weak_ptr back-edge: live count returns to 0.
// ---------------------------------------------------------------------------
namespace {
struct Link {
    static long live;
    shared_ptr<Link> next;   // strong forward edge
    weak_ptr<Link>   back;   // weak back edge — breaks the cycle
    Link()  { __atomic_add_fetch(&live, 1, __ATOMIC_SEQ_CST); }
    ~Link() { __atomic_sub_fetch(&live, 1, __ATOMIC_SEQ_CST); }
};
long Link::live = 0;
}

void test_cycle() {
    CHECK(Link::live == 0);
    {
        auto a = make_shared<Link>();
        auto b = make_shared<Link>();
        a->next = b;          // a -> b strong
        b->back = a;          // b -> a weak
        CHECK(Link::live == 2);
        CHECK(a.use_count() == 1);   // only the local `a`; b->back is weak
        CHECK(b.use_count() == 2);   // local `b` + a->next
        CHECK(!b->back.expired());
        CHECK(b->back.lock().get() == a.get());
    }
    // Had the back edge been strong, both would leak; with weak, both die.
    CHECK(Link::live == 0);
}

// ---------------------------------------------------------------------------
// make_shared single-allocation: the object sits immediately after the control
// block in ONE malloc.  We cannot count mallocs (make_shared uses malloc, not
// operator new), so we verify the CONTRACT directly: the block that make_shared
// must place at get()-sp_payload_offset has, as its FIRST word (elem-first
// layout), a pointer equal to get().  A separate allocation could not satisfy
// this.  Also checks destruction order via the live count.
// ---------------------------------------------------------------------------
void test_make_shared_single_alloc() {
    CHECK(Counted::live == 0);
    auto p = make_shared<Counted>(123);
    CHECK(p->v == 123);
    CHECK(Counted::live == 1);

    size_t off = detail::sp_payload_offset(alignof(Counted));
    // The block is a few tens of bytes below the object (24 B on LP64).
    CHECK(off >= sizeof(detail::sp_cb));
    CHECK(off < 512);                                  // "a few hundred bytes"
    void* block = static_cast<char*>(static_cast<void*>(p.get())) - off;
    void* elem_field = *static_cast<void**>(block);    // sp_cb::elem is first
    CHECK(elem_field == static_cast<void*>(p.get()));  // one allocation, adjacent

    // Destruction order: the object is destroyed exactly when the last strong
    // owner drops (before the block is freed).
    {
        auto q = p;
        CHECK(p.use_count() == 2);
    }
    CHECK(Counted::live == 1);
    p.reset();
    CHECK(Counted::live == 0);
}

// ---------------------------------------------------------------------------
// Custom deleter invoked exactly once, with the right pointer.  A default
// operator-new object is handed to a lambda deleter.
// ---------------------------------------------------------------------------
namespace {
int g_del_calls = 0;
int* g_del_ptr  = nullptr;
struct FnDeleter {
    void operator()(int* p) const { ++g_del_calls; g_del_ptr = p; delete p; }
};
}

void test_custom_deleter() {
    g_del_calls = 0;
    g_del_ptr = nullptr;
    int* raw = new int(41);
    {
        shared_ptr<int> s(raw, FnDeleter{});
        CHECK(s.get() == raw);
        CHECK(*s == 41);
        shared_ptr<int> t = s;                 // extra owner; deleter must not fire yet
        CHECK(s.use_count() == 2);
        CHECK(g_del_calls == 0);
    }
    CHECK(g_del_calls == 1);                    // exactly once
    CHECK(g_del_ptr == raw);                    // with the right pointer

    // A stateful deleter carried inline in the block.
    int sink = 0;
    {
        int* r2 = new int(8);
        shared_ptr<int> s(r2, [&sink](int* p) { sink = *p; delete p; });
    }
    CHECK(sink == 8);
}

// ---------------------------------------------------------------------------
// Single-inheritance upcast: works and preserves get() (zero offset).
// ---------------------------------------------------------------------------
namespace {
struct SBase { int x = 1; virtual ~SBase() = default; };
struct SDer : SBase { int y = 2; };
}

void test_single_inheritance_upcast() {
    shared_ptr<SDer> d = make_shared<SDer>();
    SDer* dptr = d.get();
    shared_ptr<SBase> b = d;                    // converting copy (zero offset)
    CHECK(static_cast<void*>(b.get()) == static_cast<void*>(dptr));
    CHECK(b->x == 1);
    CHECK(d.use_count() == 2);

    shared_ptr<SBase> b2 = static_pointer_cast<SBase>(d);
    CHECK(static_cast<void*>(b2.get()) == static_cast<void*>(dptr));

    // move-convert
    shared_ptr<SBase> b3 = shared_ptr<SDer>(make_shared<SDer>());
    CHECK(b3->x == 1);
}

// ---------------------------------------------------------------------------
// Multiple-inheritance offset conversion TRAPS (deviation 2).  A2 is the second
// base of Both, so it sits at a nonzero offset; converting the handle would
// mis-adjust the single stored pointer, so the library traps (SIGILL).  The
// FIRST base A1 is at offset 0 and converts freely.
// ---------------------------------------------------------------------------
namespace {
struct A1 { long a = 1; virtual ~A1() = default; };
struct A2 { long b = 2; virtual ~A2() = default; };
struct Both : A1, A2 { long c = 3; };

static int death_signal(void (*fn)()) {
    pid_t pid = fork();
    CHECK(pid >= 0);
    if (pid == 0) { fn(); _exit(0); }
    int st = 0;
    CHECK(waitpid(pid, &st, 0) == pid);
    return WIFSIGNALED(st) ? WTERMSIG(st) : -1;
}

void death_mi_convert_ctor() {
    shared_ptr<Both> both = make_shared<Both>();
    shared_ptr<A2> a2 = both;                   // nonzero offset -> trap
    (void)a2;
}
void death_mi_static_cast() {
    shared_ptr<Both> both = make_shared<Both>();
    shared_ptr<A2> a2 = static_pointer_cast<A2>(both);   // -> trap
    (void)a2;
}
void death_mi_weak_convert() {
    shared_ptr<Both> both = make_shared<Both>();
    weak_ptr<Both> wb = both;
    weak_ptr<A2> wa = wb;                        // pointer-value adjust -> trap
    (void)wa;
}
}

void test_mi_offset_traps() {
    // The zero-offset first base converts with no trap (child exits 0 -> -1).
    CHECK(death_signal([] {
        shared_ptr<Both> both = make_shared<Both>();
        shared_ptr<A1> a1 = both;               // offset 0, fine
        CHECK(static_cast<void*>(a1.get()) == static_cast<void*>(both.get()));
    }) == -1);

    CHECK(death_signal(death_mi_convert_ctor) == SIGILL);
    CHECK(death_signal(death_mi_static_cast)  == SIGILL);
    CHECK(death_signal(death_mi_weak_convert) == SIGILL);
}

// ---------------------------------------------------------------------------
// enable_shared_from_this round-trip; shared_from_this on an unowned object
// traps.
// ---------------------------------------------------------------------------
namespace {
struct Node : enable_shared_from_this<Node> {
    static long live;
    int v;
    explicit Node(int x = 0) : v(x) { __atomic_add_fetch(&live, 1, __ATOMIC_SEQ_CST); }
    ~Node() { __atomic_sub_fetch(&live, 1, __ATOMIC_SEQ_CST); }
};
long Node::live = 0;

void death_unowned_shared_from_this() {
    Node n(1);                                  // on the stack — no shared owner
    shared_ptr<Node> s = n.shared_from_this();  // weak_this_ expired -> trap
    (void)s;
}
}

void test_enable_shared_from_this() {
    CHECK(Node::live == 0);
    {
        shared_ptr<Node> p = make_shared<Node>(55);
        CHECK(p.use_count() == 1);
        shared_ptr<Node> q = p->shared_from_this();
        CHECK(q.get() == p.get());
        CHECK(p.use_count() == 2);
        CHECK(q->v == 55);

        weak_ptr<Node> w = p->weak_from_this();
        CHECK(!w.expired());
        CHECK(w.lock().get() == p.get());

        // const overload
        shared_ptr<const Node> cp = static_cast<const Node&>(*p).shared_from_this();
        CHECK(cp.get() == p.get());
    }
    CHECK(Node::live == 0);

    // Unowned object: shared_from_this must trap.
    CHECK(death_signal(death_unowned_shared_from_this) == SIGILL);
    CHECK(Node::live == 0);
}

// ---------------------------------------------------------------------------
// unique_ptr -> shared_ptr, including a custom deleter carried across.
// ---------------------------------------------------------------------------
namespace {
int g_uq_del = 0;
struct UqDeleter { void operator()(Counted* p) const { ++g_uq_del; delete p; } };
}

void test_unique_to_shared() {
    CHECK(Counted::live == 0);
    {
        unique_ptr<Counted> u(new Counted(9));
        Counted* raw = u.get();
        shared_ptr<Counted> s = std::move(u);   // routes through the pointer ctor
        CHECK(!u);
        CHECK(s.get() == raw);
        CHECK(s->v == 9);
        CHECK(s.use_count() == 1);
        CHECK(Counted::live == 1);
    }
    CHECK(Counted::live == 0);

    // unique_ptr with a custom deleter -> deleter path.
    g_uq_del = 0;
    {
        unique_ptr<Counted, UqDeleter> u(new Counted(3), UqDeleter{});
        shared_ptr<Counted> s = std::move(u);
        CHECK(s->v == 3);
    }
    CHECK(g_uq_del == 1);
    CHECK(Counted::live == 0);

    // assignment form
    {
        shared_ptr<Counted> s;
        s = unique_ptr<Counted>(new Counted(4));
        CHECK(s->v == 4);
    }
    CHECK(Counted::live == 0);
}

// ---------------------------------------------------------------------------
// nullptr / empty semantics: comparisons and owner_before.
// ---------------------------------------------------------------------------
void test_nullptr_empty() {
    shared_ptr<int> e;
    shared_ptr<int> e2(nullptr);
    CHECK(!e);
    CHECK(e == nullptr);
    CHECK(nullptr == e);
    CHECK(e.get() == nullptr);
    CHECK(e.use_count() == 0);
    CHECK(e == e2);
    CHECK(!(e != e2));

    auto a = make_shared<int>(1);
    auto b = make_shared<int>(2);
    CHECK(a != b);
    CHECK(a != nullptr);
    CHECK((a <=> nullptr) != 0);

    // owner_before is a strict weak order over the control blocks: exactly one
    // of a<b, b<a holds for distinct owners; neither for shared ownership.
    CHECK(a.owner_before(b) != b.owner_before(a));
    auto a2 = a;
    CHECK(!a.owner_before(a2) && !a2.owner_before(a));
    weak_ptr<int> wa = a;
    CHECK(!a.owner_before(wa) && !wa.owner_before(a));

    owner_less<shared_ptr<int>> less;
    CHECK(less(a, b) != less(b, a));
    owner_less<void> tless;
    CHECK(tless(a, b) != tless(b, a));

    // Empty owner_before empty is false; empty precedes any real block or not,
    // consistently.
    CHECK(!e.owner_before(e2) && !e2.owner_before(e));
}

// ---------------------------------------------------------------------------
// 20k randomized stress: a pool of shared_ptr slots over a set of make_shared
// Counted objects, mirrored against a manual reference-count array.  After
// every operation the number of objects with a positive mirror count must equal
// Counted::live exactly.
// ---------------------------------------------------------------------------
namespace {
uint32_t rng = 0x12345678u;
uint32_t rnd() { rng = rng * 1664525u + 1013904223u; return rng; }

constexpr int H = 64;      // handle slots
constexpr int NOBJ = 48;   // object identities
}

void test_stress() {
    CHECK(Counted::live == 0);
    static shared_ptr<Counted> slot[H];
    static int sid[H];                  // object id owned by slot, or -1
    static int refc[NOBJ];              // mirror: strong owners per object id
    for (int i = 0; i < H; ++i) sid[i] = -1;
    for (int i = 0; i < NOBJ; ++i) refc[i] = 0;

    auto live_from_mirror = [&]() -> long {
        long n = 0;
        for (int i = 0; i < NOBJ; ++i) if (refc[i] > 0) ++n;
        return n;
    };

    for (int iter = 0; iter < 20000; ++iter) {
        uint32_t op = rnd() % 5;
        switch (op) {
        case 0: {  // create into an empty slot with a fresh (dead) object id
            int dst = rnd() % H;
            if (sid[dst] != -1) break;
            int id = -1;
            for (int t = 0, i = rnd() % NOBJ; t < NOBJ; ++t, i = (i + 1) % NOBJ)
                if (refc[i] == 0) { id = i; break; }
            if (id == -1) break;
            slot[dst] = make_shared<Counted>(id);
            sid[dst] = id;
            refc[id] = 1;
            break;
        }
        case 1: {  // shared copy: dst(empty) = src(non-empty)
            int src = rnd() % H, dst = rnd() % H;
            if (sid[src] == -1 || sid[dst] != -1 || src == dst) break;
            slot[dst] = slot[src];
            sid[dst] = sid[src];
            ++refc[sid[src]];
            CHECK(slot[dst]->v == sid[dst]);
            break;
        }
        case 2: {  // move: dst(empty) = move(src(non-empty))
            int src = rnd() % H, dst = rnd() % H;
            if (sid[src] == -1 || sid[dst] != -1 || src == dst) break;
            slot[dst] = std::move(slot[src]);
            sid[dst] = sid[src];
            sid[src] = -1;                 // refc unchanged: ownership transferred
            CHECK(slot[dst]->v == sid[dst]);
            break;
        }
        case 3: {  // reset a non-empty slot
            int s = rnd() % H;
            if (sid[s] == -1) break;
            int id = sid[s];
            slot[s].reset();
            sid[s] = -1;
            --refc[id];
            break;
        }
        case 4: {  // weak lock: must succeed iff the object is still alive
            int s = rnd() % H;
            if (sid[s] == -1) break;
            weak_ptr<Counted> w = slot[s];
            CHECK(!w.expired());
            shared_ptr<Counted> got = w.lock();
            CHECK(got);                    // slot still owns it, so lock succeeds
            CHECK(got->v == sid[s]);
            CHECK(got.use_count() >= 2);   // slot + got
            break;
        }
        }
        CHECK(Counted::live == live_from_mirror());
    }

    // Tear down: every slot released, every object destroyed.
    for (int i = 0; i < H; ++i) slot[i].reset();
    CHECK(Counted::live == 0);
}

// ---------------------------------------------------------------------------
// Multithreaded sanity: 4 threads x 100k retain/release on shared copies of the
// SAME shared_ptr, plus a concurrent weak lock.  Concurrent COPIES (reads) of
// one shared_ptr instance are legal and route through the atomic count kernels;
// after join the strong count must be exactly the surviving owners.
// ---------------------------------------------------------------------------
namespace {
constexpr int NTHREAD = 4;
constexpr int NITER = 100000;

struct WorkerArg { shared_ptr<Counted>* p; };

void* worker(void* argp) {
    WorkerArg* a = static_cast<WorkerArg*>(argp);
    weak_ptr<Counted> w = *a->p;
    for (int i = 0; i < NITER; ++i) {
        shared_ptr<Counted> c = *a->p;   // retain (atomic)
        shared_ptr<Counted> l = w.lock();// concurrent weak lock
        // both released here
    }
    return nullptr;
}
}

void test_threads() {
    CHECK(Counted::live == 0);
    shared_ptr<Counted> p = make_shared<Counted>(1);
    CHECK(p.use_count() == 1);

    pthread_t th[NTHREAD];
    WorkerArg args[NTHREAD];
    for (int i = 0; i < NTHREAD; ++i) {
        args[i].p = &p;
        CHECK(pthread_create(&th[i], nullptr, worker, &args[i]) == 0);
    }
    for (int i = 0; i < NTHREAD; ++i)
        CHECK(pthread_join(th[i], nullptr) == 0);

    CHECK(p.use_count() == 1);            // every retain was matched by a release
    CHECK(Counted::live == 1);
    p.reset();
    CHECK(Counted::live == 0);
}

int main() {
    test_lifecycle();
    test_weak();
    test_cycle();
    test_make_shared_single_alloc();
    test_custom_deleter();
    test_single_inheritance_upcast();
    test_mi_offset_traps();
    test_enable_shared_from_this();
    test_unique_to_shared();
    test_nullptr_empty();
    test_stress();
    test_threads();
    return 0;
}
