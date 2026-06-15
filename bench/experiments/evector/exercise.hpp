// exercise.hpp — identical driving code for both containers, so the only
// difference measured is the container's own instantiated code.
//
// run_all<V>() exercises V<T> over a spread of element types (trivial, POD,
// string-bearing, custom-dtor) — the situation where per-type instantiation
// cost dominates and a type-erased core should pay off.
#pragma once
#include <string>

namespace exr {

struct Pod2 { double x, y; };

struct WithStr { std::string s; int n; };

struct WithDtor {
    int* p;
    WithDtor() : p(new int(7)) {}
    WithDtor(const WithDtor& o) : p(new int(*o.p)) {}
    WithDtor(WithDtor&& o) noexcept : p(o.p) { o.p = nullptr; }
    WithDtor& operator=(const WithDtor&) = delete;
    ~WithDtor() { delete p; }
};

template<template<class> class V, class T>
long exercise_one(const T& sample) {
    V<T> v;
    for (int i = 0; i < 200; ++i) v.push_back(sample);   // triggers several growths
    long acc = static_cast<long>(v.size());
    for (std::size_t i = 0; i < v.size(); ++i) acc ^= reinterpret_cast<long>(&v[i]) & 1;
    return acc;
}

template<template<class> class V>
long run_all() {
    long a = 0;
    a += exercise_one<V, int>(1);
    a += exercise_one<V, long>(2L);
    a += exercise_one<V, long long>(3);
    a += exercise_one<V, double>(4.0);
    a += exercise_one<V, float>(5.0f);
    a += exercise_one<V, char>(6);
    a += exercise_one<V, Pod2>(Pod2{1, 2});
    a += exercise_one<V, std::string>(
        std::string("a longish string value to keep it off the SSO fast path"));
    a += exercise_one<V, WithStr>(WithStr{std::string("x"), 9});
    a += exercise_one<V, WithDtor>(WithDtor{});
    return a;
}

} // namespace exr
