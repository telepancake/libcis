// vec_many — artificial code-size probe: std::vector instantiated over MANY
// distinct element types. Each type has a std::string member (so it is NOT
// trivially relocatable, exercising the element-by-element move/copy paths
// rather than a memcpy) and a different-sized pad array (so the compiler cannot
// fold the distinct vector<> instantiations together via ICF). This is the
// workload that a type-erased container would aim to shrink: N copies of the
// same heavy per-element machinery, one per element type.
#include <vector>
#include <string>
#include <utility>

template <int N>
struct Ty {
    char pad[N * 3 + 1];
    std::string s;
    explicit Ty(int i) : s("x") { for (char& c : pad) c = static_cast<char>(i); }
};

template <class T>
__attribute__((noinline)) void exercise(const T& z) {
    std::vector<T> v;
    for (int i = 0; i < 60; ++i) v.push_back(z);   // grow
    v.insert(v.begin() + 1, z);                     // shift + grow-with-gap
    v.erase(v.begin());                             // shift down
    v.resize(30, z);                                // shrink (destroy tail)
    std::vector<T> w(v);                            // copy-construct range
    w = v;                                          // copy-assign range
    volatile auto sink = v.size() + w.size();
    (void)sink;
}

template <int... Ns>
void all(std::integer_sequence<int, Ns...>) {
    (exercise<Ty<Ns>>(Ty<Ns>(Ns)), ...);
}

int main() {
    all(std::make_integer_sequence<int, 16>{});
    return 0;
}
