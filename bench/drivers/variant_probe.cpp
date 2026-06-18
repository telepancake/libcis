<<<<<<< HEAD
// variant_probe — artificial code-size probe: std::variant instantiated over
// three distinct flavors of a `variant<int, string, double, vector<int>>`-shaped
// type. The bench measures whether stage 6's `sum_ops` vocab (replacing the
// per-(Visitor, Ts...) fmatrix for lifecycle modifiers) actually shrinks
// programs that mix > 3 variant flavors.
//
// Each flavor differs in its alternative-type pack so the compiler cannot
// merge the instantiations under ICF. The exercised surface is the lifecycle
// modifiers stage 6 targets: copy ctor, move ctor, copy-assign, move-assign,
// destructor, emplace (cross-index). std::visit and comparisons are left for
// per-Sig template paths.
#include <variant>
#include <vector>
#include <string>
#include <utility>

// Three "shapes" that differ in their alternative type packs.
struct Sa { int x; };
struct Sb { double x; };
struct Sc { long x; };

using V1 = std::variant<int, std::string, double, std::vector<int>>;
using V2 = std::variant<Sa, std::string, double, std::vector<long>>;
using V3 = std::variant<Sb, std::string, Sc, std::vector<double>>;

template <class V, class Alt0, class Alt1, class Alt3>
__attribute__((noinline)) void exercise(Alt0 a0, Alt1 a1, Alt3 a3) {
    V v(a0);
    V w(a1);
    V x(a3);
    V copy_a = v;                       // copy ctor
    V copy_b = w;                       // copy ctor (different index)
    V move_a = std::move(x);            // move ctor
    copy_a = w;                         // copy-assign, cross-index
    copy_b = std::move(move_a);         // move-assign
    v.template emplace<1>(a1);          // emplace, cross-index
    v = std::move(copy_b);
    volatile auto sink = v.index() + copy_a.index();
=======
// variant_probe — artificial code-size probe: three distinct variant flavors
// over a mix of trivial and non-trivial alternatives. Each flavor exercises
// the variant lifecycle paths (default ctor, converting ctor, copy, move,
// assignment, swap) so that whichever machinery the library picks for those
// modifiers — fmatrix-per-flavor vs sum_ops-per-flavor — shows up in .text.
//
// The shape is the one §6 stage 0 of notes/cores-design.md calls for:
// variant<int, string, double, vector<int>>-flavored types, three flavors
// distinct enough that the compiler cannot fold them via ICF (different
// alternative type sets and different "extra" type to break a fold).
#include <variant>
#include <string>
#include <vector>
#include <utility>

template <int N>
struct Extra {
    char pad[N * 7 + 1];
    std::string s;
    Extra() : s("x") { for (char& c : pad) c = static_cast<char>(N); }
    Extra(const Extra&) = default;
    Extra(Extra&&) = default;
    Extra& operator=(const Extra&) = default;
    Extra& operator=(Extra&&) = default;
};

template <class V, class T>
__attribute__((noinline)) void exercise(T&& seed) {
    V v;                                            // default ctor
    v = std::forward<T>(seed);                      // converting assign
    V v2(v);                                        // copy ctor
    V v3(std::move(v));                             // move ctor
    v = v2;                                         // copy assign
    v = std::move(v3);                              // move assign
    V v4;
    v.swap(v4);                                     // swap (same/different idx)
    v4 = v2;
    v.swap(v4);
    volatile auto sink = v.index() + v2.index() + v4.index();
>>>>>>> ec11a75 (cores: stage 6 — sum_ops vocab + variant lifecycle ported)
    (void)sink;
}

int main() {
<<<<<<< HEAD
    exercise<V1, int, std::string, std::vector<int>>(
        1, std::string("a"), std::vector<int>{1, 2, 3});
    exercise<V2, Sa, std::string, std::vector<long>>(
        Sa{2}, std::string("b"), std::vector<long>{4L, 5L});
    exercise<V3, Sb, std::string, std::vector<double>>(
        Sb{3.0}, std::string("c"), std::vector<double>{6.0, 7.0});
=======
    using V0 = std::variant<int, std::string, double, std::vector<int>>;
    using V1 = std::variant<int, std::string, double, std::vector<int>, Extra<1>>;
    using V2 = std::variant<long, std::string, float, std::vector<long>, Extra<2>>;
    exercise<V0>(std::string("v0"));
    exercise<V1>(std::string("v1"));
    exercise<V2>(std::string("v2"));
>>>>>>> ec11a75 (cores: stage 6 — sum_ops vocab + variant lifecycle ported)
    return 0;
}
