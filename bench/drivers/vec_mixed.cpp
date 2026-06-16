// vec_mixed — artificial code-size probe: std::vector over a handful of common,
// dissimilar element types (trivial scalars, a heap type, and a struct mixing
// both). A smaller, more "realistic" companion to vec_many: a typical program
// uses a few vector element types, not sixteen. Exercises the same mutating
// surface (push_back/insert/erase/resize/copy/assign) per type.
#include <vector>
#include <string>

struct Big {
    double a, b, c;
    std::string s;
    explicit Big(int i) : a(i), b(i), c(i), s("x") {}
};

template <class T>
__attribute__((noinline)) void exercise(const T& z) {
    std::vector<T> v;
    for (int i = 0; i < 100; ++i) v.push_back(z);
    v.insert(v.begin() + 1, z);
    v.erase(v.begin());
    v.resize(50, z);
    std::vector<T> w(v);
    w = v;
    volatile auto sink = v.size() + w.size();
    (void)sink;
}

int main() {
    exercise<int>(1);
    exercise<long>(2L);
    exercise<double>(3.0);
    exercise<std::string>(std::string("y"));
    exercise<Big>(Big(4));
    return 0;
}
