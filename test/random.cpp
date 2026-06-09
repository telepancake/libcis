// test/random.cpp -- basic sanity tests for <random>
// Each test function crashes (via CHECK) on failure; passing run is silent.
#include "test.h"
#include <random>
#include <cmath>

// mt19937 standard validation: default seed, 10000th value == 4123659995
void test_random_mt19937_standard() {
    std::mt19937 rng;
    std::mt19937::result_type v = 0;
    for (int i = 0; i < 10000; ++i)
        v = rng();
    CHECK(v == 4123659995u);
}

// uniform_int_distribution must stay in [a, b]
void test_random_uniform_int() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(10, 20);
    for (int i = 0; i < 1000; ++i) {
        int v = dist(rng);
        CHECK(v >= 10 && v <= 20);
    }
}

// uniform_real_distribution must stay in [a, b)
void test_random_uniform_real() {
    std::mt19937 rng(1234);
    std::uniform_real_distribution<double> dist(2.0, 5.0);
    for (int i = 0; i < 1000; ++i) {
        double v = dist(rng);
        CHECK(v >= 2.0 && v < 5.0);
    }
}

// seed reproducibility: same seed => same sequence
void test_random_seed_reproducibility() {
    std::mt19937 rng1(999), rng2(999);
    for (int i = 0; i < 100; ++i)
        CHECK(rng1() == rng2());
    // Different seeds differ
    std::mt19937 rng3(1), rng4(2);
    bool any_diff = false;
    for (int i = 0; i < 20; ++i)
        any_diff |= (rng3() != rng4());
    CHECK(any_diff);
}

// bernoulli_distribution: p=0 => always false, p=1 => always true, p=0.5 => mix
void test_random_bernoulli() {
    std::mt19937 rng(77);
    std::bernoulli_distribution always_false(0.0);
    std::bernoulli_distribution always_true(1.0);
    std::bernoulli_distribution half(0.5);
    for (int i = 0; i < 100; ++i)
        CHECK(!always_false(rng));
    for (int i = 0; i < 100; ++i)
        CHECK(always_true(rng));
    int trues = 0;
    for (int i = 0; i < 1000; ++i)
        if (half(rng)) ++trues;
    // expect roughly 500; check at least some of each
    CHECK(trues > 350 && trues < 650);
}

// normal_distribution: mean and stddev are roughly correct
void test_random_normal() {
    std::mt19937 rng(555);
    std::normal_distribution<double> dist(0.0, 1.0);
    double sum = 0.0, sum2 = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        sum += v;
        sum2 += v * v;
    }
    double mean = sum / N;
    double var  = sum2 / N - mean * mean;
    // mean should be near 0, variance near 1
    CHECK(mean > -0.1 && mean < 0.1);
    CHECK(var > 0.9 && var < 1.1);
}

// poisson_distribution: mean ~ lambda
void test_random_poisson() {
    std::mt19937 rng(321);
    std::poisson_distribution<int> dist(5.0);
    long long sum = 0;
    const int N = 10000;
    for (int i = 0; i < N; ++i)
        sum += dist(rng);
    double mean = (double)sum / N;
    // mean should be near 5
    CHECK(mean > 4.5 && mean < 5.5);
}

// discrete_distribution: {0, 1, 0} => only index 1 ever returned
void test_random_discrete() {
    std::mt19937 rng(888);
    std::discrete_distribution<int> dist({0.0, 1.0, 0.0});
    for (int i = 0; i < 100; ++i)
        CHECK(dist(rng) == 1);
}

// gamma_distribution: mean ~ alpha*beta, values >= 0
void test_random_gamma() {
    std::mt19937 rng(101);
    std::gamma_distribution<double> dist(2.0, 3.0); // mean=6, var=18
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v >= 0.0);
        sum += v;
    }
    double mean = sum / N;
    CHECK(mean > 5.0 && mean < 7.0);
    // param round-trip
    auto p = dist.param();
    CHECK(p.alpha() == 2.0 && p.beta() == 3.0);
    dist.param(std::gamma_distribution<double>::param_type(1.0, 1.0));
    CHECK(dist.alpha() == 1.0);
}

// binomial_distribution: values in [0, t], mean ~ t*p
void test_random_binomial() {
    std::mt19937 rng(202);
    std::binomial_distribution<int> dist(20, 0.5); // mean=10
    long long sum = 0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        int v = dist(rng);
        CHECK(v >= 0 && v <= 20);
        sum += v;
    }
    double mean = (double)sum / N;
    CHECK(mean > 9.0 && mean < 11.0);
    // param round-trip
    auto p = dist.param();
    CHECK(p.t() == 20 && p.p() == 0.5);
}

// negative_binomial_distribution: values >= 0, mean ~ k*(1-p)/p
void test_random_negative_binomial() {
    std::mt19937 rng(303);
    // k=5, p=0.5 => mean = k*(1-p)/p = 5
    std::negative_binomial_distribution<int> dist(5, 0.5);
    long long sum = 0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        int v = dist(rng);
        CHECK(v >= 0);
        sum += v;
    }
    double mean = (double)sum / N;
    CHECK(mean > 4.0 && mean < 6.0);
    // param round-trip
    auto p = dist.param();
    CHECK(p.k() == 5 && p.p() == 0.5);
}

// geometric_distribution: values >= 0, mean ~ (1-p)/p
void test_random_geometric() {
    std::mt19937 rng(404);
    // p=0.5 => mean = (1-0.5)/0.5 = 1
    std::geometric_distribution<int> dist(0.5);
    long long sum = 0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        int v = dist(rng);
        CHECK(v >= 0);
        sum += v;
    }
    double mean = (double)sum / N;
    CHECK(mean > 0.7 && mean < 1.3);
    // param round-trip
    auto p = dist.param();
    CHECK(p.p() == 0.5);
}

// weibull_distribution: values >= 0, mean = b*Gamma(1+1/a)
void test_random_weibull() {
    std::mt19937 rng(505);
    std::weibull_distribution<double> dist(1.0, 2.0); // mean=2 (exponential with scale 2)
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v >= 0.0);
        sum += v;
    }
    double mean = sum / N;
    CHECK(mean > 1.6 && mean < 2.4);
    // param round-trip
    auto p = dist.param();
    CHECK(p.a() == 1.0 && p.b() == 2.0);
}

// extreme_value_distribution: finite values, mean ~ a + b*euler_mascheroni
void test_random_extreme_value() {
    std::mt19937 rng(606);
    std::extreme_value_distribution<double> dist(0.0, 1.0); // mean ~ 0.5772
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i)
        sum += dist(rng);
    double mean = sum / N;
    CHECK(mean > 0.3 && mean < 0.9);
    // param round-trip
    auto p = dist.param();
    CHECK(p.a() == 0.0 && p.b() == 1.0);
}

// lognormal_distribution: values > 0, mean = exp(m + s^2/2)
void test_random_lognormal() {
    std::mt19937 rng(707);
    std::lognormal_distribution<double> dist(0.0, 1.0); // mean = exp(0.5) ~ 1.6487
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v > 0.0);
        sum += v;
    }
    double mean = sum / N;
    CHECK(mean > 1.3 && mean < 2.0);
    // param round-trip
    auto p = dist.param();
    CHECK(p.m() == 0.0 && p.s() == 1.0);
}

// chi_squared_distribution: values >= 0, mean = n
void test_random_chi_squared() {
    std::mt19937 rng(808);
    std::chi_squared_distribution<double> dist(4.0); // mean=4
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v >= 0.0);
        sum += v;
    }
    double mean = sum / N;
    CHECK(mean > 3.5 && mean < 4.5);
    // param round-trip
    auto p = dist.param();
    CHECK(p.n() == 4.0);
}

// cauchy_distribution: median ~ a (mean undefined, just check it generates)
void test_random_cauchy() {
    std::mt19937 rng(909);
    std::cauchy_distribution<double> dist(0.0, 1.0);
    int cnt = 0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        if (v > -10.0 && v < 10.0) ++cnt;
    }
    // Most values should be within -10..10 for Cauchy(0,1)
    CHECK(cnt > 8000);
    // param round-trip
    auto p = dist.param();
    CHECK(p.a() == 0.0 && p.b() == 1.0);
}

// fisher_f_distribution: values >= 0, mean = n/(n-2) for n>2
void test_random_fisher_f() {
    std::mt19937 rng(1010);
    std::fisher_f_distribution<double> dist(10.0, 20.0); // mean = 20/(20-2) ~ 1.111
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v >= 0.0);
        sum += v;
    }
    double mean = sum / N;
    CHECK(mean > 0.8 && mean < 1.5);
    // param round-trip
    auto p = dist.param();
    CHECK(p.m() == 10.0 && p.n() == 20.0);
}

// student_t_distribution: mean ~ 0, symmetric
void test_random_student_t() {
    std::mt19937 rng(1111);
    std::student_t_distribution<double> dist(10.0); // mean=0
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i)
        sum += dist(rng);
    double mean = sum / N;
    CHECK(mean > -0.2 && mean < 0.2);
    // param round-trip
    auto p = dist.param();
    CHECK(p.n() == 10.0);
}

// piecewise_constant_distribution: values in [0,2), uniform weights => uniform
void test_random_piecewise_constant() {
    std::mt19937 rng(1212);
    double bounds[] = {0.0, 1.0, 2.0};
    double weights[] = {1.0, 1.0};
    std::piecewise_constant_distribution<double> dist(bounds, bounds + 3, weights);
    int below = 0, above = 0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v >= 0.0 && v < 2.0);
        if (v < 1.0) ++below; else ++above;
    }
    // roughly half in each interval
    CHECK(below > 4000 && below < 6000);
    CHECK(above > 4000 && above < 6000);
    // param round-trip
    auto p = dist.param();
    auto iv = p.intervals();
    CHECK(iv.size() == 3);
}

// piecewise_linear_distribution: triangular density 0->1->0 on [0,2]
void test_random_piecewise_linear() {
    std::mt19937 rng(1313);
    double bounds[]  = {0.0, 1.0, 2.0};
    double weights[] = {0.0, 1.0, 0.0};
    std::piecewise_linear_distribution<double> dist(bounds, bounds + 3, weights);
    double sum = 0.0;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        double v = dist(rng);
        CHECK(v >= 0.0 && v <= 2.0);
        sum += v;
    }
    // mean of triangular(0,1,2) = 1.0
    double mean = sum / N;
    CHECK(mean > 0.9 && mean < 1.1);
    // param round-trip
    auto p = dist.param();
    auto iv = p.intervals();
    CHECK(iv.size() == 3);
}
