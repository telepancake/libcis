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
    (void)sink;
}

int main() {
    exercise<V1, int, std::string, std::vector<int>>(
        1, std::string("a"), std::vector<int>{1, 2, 3});
    exercise<V2, Sa, std::string, std::vector<long>>(
        Sa{2}, std::string("b"), std::vector<long>{4L, 5L});
    exercise<V3, Sb, std::string, std::vector<double>>(
        Sb{3.0}, std::string("c"), std::vector<double>{6.0, 7.0});
    return 0;
}
