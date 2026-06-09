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
