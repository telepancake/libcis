// vector_ops.cpp - front insert/erase (full shift) over 8 distinct element types
// that the relocatability trait DETECTS (string + vector<X> instantiations:
// non-trivially-copyable but opted-in relocatable). These exercise the heavy
// shift, where the typed path is a per-type move-assign loop and the relocatable
// path is one memmove.
#include <vector>
#include <string>

template<class E>
long exercise(const E& sample) {
    std::vector<E> v;
    for (int i = 0; i < 50; ++i) v.insert(v.begin(), sample);   // front insert: full shift
    for (int i = 0; i < 20; ++i) v.erase(v.begin());             // front erase: full shift
    v.insert(v.begin() + 5, sample);                            // middle insert
    return static_cast<long>(v.size());
}

volatile long sink;
int main() {
    sink += exercise(std::string("a relocatable string value, longish"));
    sink += exercise(std::vector<int>{1, 2, 3});
    sink += exercise(std::vector<long>{1, 2});
    sink += exercise(std::vector<double>{1.0});
    sink += exercise(std::vector<char>{'a', 'b'});
    sink += exercise(std::vector<std::string>{"x"});
    sink += exercise(std::vector<short>{1});
    sink += exercise(std::vector<void*>{nullptr});
    return 0;
}
