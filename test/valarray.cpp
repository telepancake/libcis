#include <valarray>
#include <cmath>
#include "test.h"

void test_valarray_elementwise() {
    std::valarray<double> a = {1.0, 2.0, 3.0, 4.0};
    std::valarray<double> b = {10.0, 20.0, 30.0, 40.0};

    // element-wise +
    std::valarray<double> c = a + b;
    CHECK(c.size() == 4);
    CHECK(c[0] == 11.0);
    CHECK(c[1] == 22.0);
    CHECK(c[2] == 33.0);
    CHECK(c[3] == 44.0);

    // element-wise *
    std::valarray<double> d = a * b;
    CHECK(d[0] == 10.0);
    CHECK(d[1] == 40.0);
    CHECK(d[2] == 90.0);
    CHECK(d[3] == 160.0);
}

void test_valarray_scalar_ops() {
    std::valarray<double> a = {2.0, 4.0, 6.0, 8.0};

    std::valarray<double> b = a * 2.0;
    CHECK(b[0] == 4.0);
    CHECK(b[3] == 16.0);

    std::valarray<double> c = 1.0 + a;
    CHECK(c[0] == 3.0);
    CHECK(c[1] == 5.0);

    a += 1.0;
    CHECK(a[0] == 3.0);
    CHECK(a[3] == 9.0);
}

void test_valarray_sum_min_max() {
    std::valarray<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    CHECK(v.sum() == 31);
    CHECK(v.min() == 1);
    CHECK(v.max() == 9);
}

void test_valarray_apply() {
    std::valarray<double> v = {1.0, 4.0, 9.0, 16.0};
    auto result = v.apply([](double x) -> double { return x * 2.0; });
    CHECK(result[0] == 2.0);
    CHECK(result[1] == 8.0);
    CHECK(result[2] == 18.0);
    CHECK(result[3] == 32.0);
}

void test_valarray_slice_assignment() {
    std::valarray<int> v(0, 6);  // {0,0,0,0,0,0}
    // Assign to slice: elements 0,2,4 (start=0, size=3, stride=2)
    std::valarray<int> vals = {10, 20, 30};
    v[std::slice(0, 3, 2)] = vals;
    CHECK(v[0] == 10);
    CHECK(v[1] == 0);
    CHECK(v[2] == 20);
    CHECK(v[3] == 0);
    CHECK(v[4] == 30);
    CHECK(v[5] == 0);
}

void test_valarray_sqrt() {
    std::valarray<double> v = {4.0, 9.0, 16.0, 25.0};
    std::valarray<double> r = std::sqrt(v);
    CHECK(r[0] == 2.0);
    CHECK(r[1] == 3.0);
    CHECK(r[2] == 4.0);
    CHECK(r[3] == 5.0);
}
