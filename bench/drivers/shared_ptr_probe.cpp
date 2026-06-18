// shared_ptr_probe — artificial code-size probe: std::shared_ptr instantiated
// over eight distinct element types, exercising the three control-block
// flavors stage 7 (`sp_cb_ops`) replaces:
//
//   1. make_shared<T>(...)           — emplace control block (T inline)
//   2. shared_ptr<T>(new T(...))     — separate-ptr control block, default delete
//   3. shared_ptr<T[]>(new T[...],   — array control block, explicit deleter
//                      default_delete<T[]>())
//
// The two non-trivial virtual bodies in `sp_counted_*` (the
// `on_zero_shared` / `on_zero_shared_weak` slots) are the surface stage 7
// folds across template parameters; the bench measures the per-T cost of the
// vtable today so the journal entry after stage 7 can claim a real delta.
#include <memory>
#include <string>
#include <vector>
#include <utility>

// Eight T flavors. Mix triviality, allocation, and size so the per-instance
// `sp_counted_*<T,...>` vtables do not fold under ICF.
struct T0 { int a; };
struct T1 { double a; long b; };
struct T2 { char pad[7]; };
struct T3 { std::string s; T3() : s("x") {} };
struct T4 { std::vector<int> v; T4() : v{1,2,3} {} };
struct T5 { double a, b, c, d; };
struct T6 { std::string s; int n; T6() : s("y"), n(0) {} };
struct T7 { std::vector<std::string> v; T7() : v{"a","b"} {} };

template <class T>
__attribute__((noinline)) void exercise() {
    // (1) make_shared — emplace control block, T inline with the refcount.
    auto a = std::make_shared<T>();
    auto a2 = a;                       // copy: refcount bump
    auto a3 = std::move(a2);           // move: no refcount touch

    // (2) shared_ptr(new T) — separate ptr, default delete.
    std::shared_ptr<T> b(new T());
    auto b2 = b;
    b.reset();                         // drop one ref
    b2.reset();                        // drop the last ref → on_zero_shared

    // (3) shared_ptr<T[]>(new T[N], default_delete<T[]>()) — array variant.
    std::shared_ptr<T> c(new T[4](), std::default_delete<T[]>());
    auto c2 = c;
    c.reset();
    c2.reset();

    volatile auto sink = a.use_count();
    (void)sink;
}

int main() {
    exercise<T0>();
    exercise<T1>();
    exercise<T2>();
    exercise<T3>();
    exercise<T4>();
    exercise<T5>();
    exercise<T6>();
    exercise<T7>();
    return 0;
}
