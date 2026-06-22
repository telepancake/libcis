// firmware.cpp — the scalable composite image behind `make size`.
//
// One program, built repeatedly at -DNT=<n>, that instantiates and *uses* the
// library across a broad surface (vector grow/insert/erase/resize, sort, find,
// copy/assign over a non-trivially-relocatable element). This stands in for a
// real firmware image: the shared library code is linked ONCE, and N distinct
// container instantiations call into it.
//
// The metric is NOT this binary's total size. It is the SLOPE of total size as
// NT grows — the marginal flash/RAM each *additional* container instantiation
// adds. That slope cancels the one-time shared-core cost (the y-intercept), so:
//
//   * pushing logic into a shared, type-erased core  -> LOW slope  (good)
//   * inlining/duplicating logic per instantiation    -> HIGH slope (bad)
//
// It is exactly the quantity a whole-firmware build pays per extra container,
// and exactly what a "count the fixed core into every micro-benchmark" harness
// hides. Element sizes are strictly distinct (pad[N+1]) so ICF/COMDAT genuinely
// cannot fold the instantiations — the slope must be earned by the library
// sharing code, not by the linker deduplicating identical bodies.

#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <any>
#include <optional>
#include <variant>

#ifndef NT
#define NT 8
#endif

template <int N>
struct Elem {
    char pad[N + 1];               // strictly distinct size -> ICF cannot fold
    std::string s;                 // non-trivially-relocatable -> heavy move/copy paths
    explicit Elem(int i) : s(i & 1 ? "xx" : "yyyy") { for (char& c : pad) c = (char)(i + N); }
    bool operator<(const Elem& o) const { return s < o.s; }
};

// Cross-header type erasure over the SAME element type T. A real image that
// stores T in a vector also routinely funnels T through an any / optional /
// variant slot. Each of those headers needs T's copy/move/destroy leaves; if
// every vocabulary emits its OWN copy/move/destroy body for T, the marginal
// flash per added type is multiplied by the number of vocabularies. Unifying
// them onto one shared type_ops leaf set collapses that multiple. Exercising
// them here over T makes that duplication (or its removal) show in the slope.
template <class T>
__attribute__((noinline)) unsigned exercise_erased(const T& proto) {
    std::any a = proto;            // any: copy/move/destroy + type_id leaves for T
    std::any b = a;                // copy-construct erased storage
    a = std::move(b);              // move-assign erased storage
    std::optional<T> o = proto;    // optional<T>: copy/move/destroy of the value
    std::optional<T> p = o;        // copy
    o = std::move(p);              // move
    std::variant<int, T> vt = proto;   // variant: copy/move/destroy active alt
    std::variant<int, T> wt = vt;      // copy
    vt = std::move(wt);                // move
    size_t acc = 0;
    if (auto* pa = std::any_cast<T>(&a)) acc += pa->s.size();
    if (o) acc += o->s.size();
    if (auto* pv = std::get_if<1>(&vt)) acc += pv->s.size();
    return (unsigned)acc;
}

template <class T>
__attribute__((noinline)) unsigned exercise(int seed) {
    std::vector<T> v;
    for (int i = 0; i < 64; ++i) v.push_back(T(seed + i));   // grow
    v.insert(v.begin() + 1, T(seed));                         // shift + grow-with-gap
    v.erase(v.begin() + 2);                                   // shift down
    v.resize(40, T(seed));                                    // shrink (destroy tail)
    std::sort(v.begin(), v.end());                            // algorithm over T
    std::vector<T> w(v);                                      // copy-construct range
    w = v;                                                    // copy-assign range
    auto it = std::find_if(w.begin(), w.end(),
                           [](const T& x){ return x.s.size() > 3; });
    return (unsigned)(v.size() + w.size() + (it != w.end()))
           + exercise_erased<T>(v.front());
}

template <int... Ns>
__attribute__((noinline)) unsigned all(std::integer_sequence<int, Ns...>) {
    unsigned acc = 0;
    ((acc += exercise<Elem<Ns>>(Ns + 1)), ...);
    return acc;
}

#ifdef PERF
// Perf build: run the same surface in a timed loop. The harness times the
// process so a size change that wins bytes by pessimizing the shared core
// (more indirection on the hot path) shows up as a runtime regression.
#include <ctime>
int main() {
    unsigned acc = 0;
    for (int r = 0; r < PERF; ++r)
        acc += all(std::make_integer_sequence<int, NT>{});
    return (int)(acc % 7);
}
#else
int main() {
    volatile unsigned sink = all(std::make_integer_sequence<int, NT>{});
    return (int)(sink % 7);
}
#endif
