// vector_ops.cpp - exercise the HEAVY vector methods (insert/erase at front,
// which force a full shift) over many distinct, non-trivial, trivially-
// relocatable element types. This is the regime where type-erasing the shift
// into one shared size-parameterized core should pay off — unlike push_back of
// trivial scalars (vector_std), which is the worst case for it.
#include <vector>
#include <string>

namespace d {
// 8 distinct types, each non-trivial (string member -> non-trivial move/dtor)
// but trivially relocatable (no self-pointer).
struct T0 { std::string s; long a;        T0(int i=0):s(std::to_string(i)),a(i){} };
struct T1 { std::string s; double a,b;    T1(int i=0):s("t1"+std::to_string(i)),a(i),b(-i){} };
struct T2 { std::string s; int a[3];      T2(int i=0):s("t2"),a{i,i,i}{} };
struct T3 { std::string s; char a[7];     T3(int i=0):s("t3"),a{}{a[0]=(char)i;} };
struct T4 { std::string s; long a,b,c;    T4(int i=0):s("t4"),a(i),b(i),c(i){} };
struct T5 { std::vector<int> v; std::string s; T5(int i=0):v{i,i+1},s("t5"){} };
struct T6 { std::string s; short a;       T6(int i=0):s("t6sixsix"),a((short)i){} };
struct T7 { std::string s; void* p;       T7(int i=0):s("t7"),p(nullptr){} };

template<class T>
long exercise() {
    std::vector<T> v;
    for (int i = 0; i < 50; ++i) v.insert(v.begin(), T(i));   // front insert: full shift each time
    for (int i = 0; i < 20; ++i) v.erase(v.begin());           // front erase: full shift each time
    v.insert(v.begin() + 5, T(99));                            // middle insert
    return static_cast<long>(v.size());
}
} // namespace d

volatile long sink;
int main() {
    sink += d::exercise<d::T0>(); sink += d::exercise<d::T1>();
    sink += d::exercise<d::T2>(); sink += d::exercise<d::T3>();
    sink += d::exercise<d::T4>(); sink += d::exercise<d::T5>();
    sink += d::exercise<d::T6>(); sink += d::exercise<d::T7>();
    return 0;
}
